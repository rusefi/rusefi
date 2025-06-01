#!/usr/bin/env python3
"""
Script de debug para investigar problemas com o wall wetting adaptativo.
Analisa as possíveis causas por que as correções não estão acontecendo.
"""

import os
import re
from pathlib import Path

def print_header(title):
    print(f"\n{'='*60}")
    print(f"🔍 {title}")
    print(f"{'='*60}")

def check_configuration_parameters():
    """Verifica se os parâmetros de configuração do Aquino estão definidos corretamente."""
    print_header("VERIFICANDO PARÂMETROS DE CONFIGURAÇÃO")
    
    config_file = "firmware/integration/evotech_config.txt"
    if not os.path.exists(config_file):
        print("❌ ERRO: Arquivo evotech_config.txt não encontrado!")
        return False
    
    # Parâmetros obrigatórios do modelo Aquino
    required_params = [
        "wwAquinoAccelThresh",
        "wwAquinoDecelThresh", 
        "wwAquinoMapAccelThresh",
        "wwAquinoMapDecelThresh",
        "wwAquinoMinLambdaErr",
        "wwAquinoTransMinDuration",
        "wwAquinoAnalysisMaxDuration",
        "wwAquinoBetaAccelGain",
        "wwAquinoBetaDecelGain",
        "wwAquinoBetaLeanThresh",
        "wwAquinoBetaRichThresh",
        "wwAquinoTauSettleThresh",
        "wwAquinoTauIdealFactor",
        "wwAquinoTauToleranceFactor",
        "wwAquinoTauOvershootMinDur",
        "wwAquinoTauOvershootMinMag",
        "wwAquinoTauOvershootGain",
        "wwAquinoTauSlowGain",
        "wwAquinoTauFastGain"
    ]
    
    with open(config_file, 'r', encoding='utf-8') as f:
        content = f.read()
    
    missing_params = []
    found_params = {}
    
    for param in required_params:
        # Procura definição do parâmetro
        pattern = rf"float {param};.*?;.*?(\d+(?:\.\d+)?),.*?(\d+(?:\.\d+)?),.*?(-?\d+(?:\.\d+)?),.*?(-?\d+(?:\.\d+)?),.*?(\d+)"
        match = re.search(pattern, content)
        
        if match:
            # Extrai valores: scale, offset, min, max, digits
            scale, offset, min_val, max_val, digits = match.groups()
            found_params[param] = {
                'scale': float(scale),
                'offset': float(offset), 
                'min': float(min_val),
                'max': float(max_val),
                'digits': int(digits)
            }
            print(f"✅ {param}: min={min_val}, max={max_val}, scale={scale}")
        else:
            missing_params.append(param)
            print(f"❌ {param}: NÃO ENCONTRADO")
    
    if missing_params:
        print(f"\n⚠️  PARÂMETROS FALTANDO ({len(missing_params)}):")
        for param in missing_params:
            print(f"   - {param}")
        return False
    
    # Verificar valores suspeitos (thresholds zerados)
    print(f"\n📊 VERIFICANDO VALORES SUSPEITOS:")
    suspicious = []
    
    for param, values in found_params.items():
        if "Thresh" in param and values['min'] == 0 and values['max'] == 0:
            suspicious.append(param)
            print(f"⚠️  {param}: min=max=0 (threshold desabilitado?)")
        elif "Gain" in param and values['min'] == 0 and values['max'] == 0:
            suspicious.append(param)
            print(f"⚠️  {param}: min=max=0 (ganho zerado?)")
    
    if not suspicious:
        print("✅ Todos os valores parecem razoáveis")
    
    return len(missing_params) == 0

def check_enable_flags():
    """Verifica flags de habilitação do sistema."""
    print_header("VERIFICANDO FLAGS DE HABILITAÇÃO")
    
    # Verifica no evotech_config.txt
    config_file = "firmware/integration/evotech_config.txt"
    if os.path.exists(config_file):
        with open(config_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Procura flags relacionadas
        enable_flags = [
            "complexWallModel",
            "wwEnableAdaptiveLearning",
            "wwMinCoolantTemp"
        ]
        
        for flag in enable_flags:
            if flag in content:
                print(f"✅ {flag}: ENCONTRADO na configuração")
                # Extrai linha para mostrar configuração
                for line in content.split('\n'):
                    if flag in line and not line.strip().startswith('//'):
                        print(f"   → {line.strip()}")
                        break
            else:
                print(f"❌ {flag}: NÃO ENCONTRADO")
    else:
        print("❌ Arquivo de configuração não encontrado")

def analyze_conditions_chain():
    """Analisa a cadeia de condições que podem impedir as correções."""
    print_header("ANALISANDO CADEIA DE CONDIÇÕES")
    
    print("🔗 SEQUÊNCIA DE VERIFICAÇÕES NO onFastCallback():")
    print()
    
    conditions = [
        ("1. Cranking", "!engine->rpmCalculator.isCranking()", "Se motor girando de partida → abort"),
        ("2. Tau válido", "tau >= 0.001f", "Se tau muito baixo → abort"), 
        ("3. Beta válido", "beta >= 0.01f", "Se beta muito baixo → abort"),
        ("4. RPM mínimo", "rpm >= 100", "Se RPM muito baixo → abort"),
        ("5. Produto RPM×tau", "rpm * tau >= 0.001f", "Se produto muito baixo → abort"),
        ("6. Beta < Alpha", "beta <= alpha", "Se sistema subamortecido → clamp beta"),
        ("7. Wall wetting habilitado", "wwEnableAdaptiveLearning == true", "Se adaptativo desabilitado → abort"),
        ("8. Motor aquecido", "CLT >= wwMinCoolantTemp", "Se motor frio → abort"),
        ("9. MAP válido", "MAP sensor válido", "Se MAP inválido → abort"),
        ("10. TPS válido", "TPS sensor válido", "Se TPS inválido → abort (crítico para Aquino)"),
        ("11. Lambda válido", "0.5 < lambda < 3.5", "Se lambda inválido → não coleta dados"),
        ("12. Target lambda válido", "0.5 < targetLambda < 1.5", "Se target inválido → não coleta dados"),
    ]
    
    for step, condition, description in conditions:
        print(f"{step}: {condition}")
        print(f"   {description}")
        print()
    
    print("🎯 CONDIÇÕES PARA DETECÇÃO DE TRANSIENTES (Aquino):")
    print()
    print("MAP Aceleração: loadDerivative > wwAquinoMapAccelThresh")
    print("MAP Desaceleração: loadDerivative < wwAquinoMapDecelThresh") 
    print("TPS Aceleração: tpsDerivative > wwAquinoAccelThresh")
    print("TPS Desaceleração: tpsDerivative < wwAquinoDecelThresh")
    print()
    print("⚠️  SE NENHUM TRANSIENTE FOR DETECTADO → Nenhuma correção acontece")

def check_potential_issues():
    """Identifica problemas potenciais que podem impedir as correções."""
    print_header("PROBLEMAS POTENCIAIS IDENTIFICADOS")
    
    issues = [
        {
            'title': 'Thresholds muito altos',
            'description': 'Se wwAquino*Thresh muito altos, transientes normais não são detectados',
            'solution': 'Reduzir thresholds: AccelThresh=30%/s, MapAccelThresh=20kPa/s'
        },
        {
            'title': 'Ganhos zerados', 
            'description': 'Se wwAquino*Gain = 0, correções são calculadas como zero',
            'solution': 'Configurar ganhos: BetaGain=0.1, TauGain=0.1'
        },
        {
            'title': 'Temperatura muito alta',
            'description': 'Se wwMinCoolantTemp muito alto, sistema nunca habilita',
            'solution': 'Configurar temperatura razoável: 60-80°C'
        },
        {
            'title': 'Sensores inválidos',
            'description': 'TPS, MAP ou Lambda inválidos impedem funcionamento',
            'solution': 'Verificar calibração e wiring dos sensores'
        },
        {
            'title': 'Lambda fora da faixa',
            'description': 'Lambda fora de 0.5-3.5 ou target fora de 0.5-1.5',
            'solution': 'Verificar calibração do sensor lambda e target'
        },
        {
            'title': 'Transientes muito curtos',
            'description': 'Se transientes < wwAquinoTransMinDuration, são ignorados',
            'solution': 'Reduzir duração mínima para 0.2s'
        },
        {
            'title': 'Tabelas não inicializadas',
            'description': 'Tabelas wwBetaCorrection/wwTauCorrection podem estar com valores inválidos',
            'solution': 'Inicializar todas as células com 1.0 (sem correção)'
        },
        {
            'title': 'Buffer underrun',
            'description': 'Buffers podem não estar sendo preenchidos adequadamente',
            'solution': 'Verificar indexação dos buffers circulares'
        }
    ]
    
    for i, issue in enumerate(issues, 1):
        print(f"{i}. 🚨 {issue['title']}")
        print(f"   Problema: {issue['description']}")
        print(f"   Solução: {issue['solution']}")
        print()

def suggest_debug_code():
    """Sugere código de debug para adicionar ao wall_fuel.cpp."""
    print_header("CÓDIGO DE DEBUG SUGERIDO")
    
    debug_code = '''
// Adicionar no início do onFastCallback() (após verificações básicas):
static int debugCounter = 0;
debugCounter++;

if (debugCounter % 200 == 0) { // Debug a cada 1 segundo (200Hz)
    efiPrintf("WW_DEBUG: enable=%d, adaptive=%d, CLT=%.1f, RPM=%.0f", 
              m_enable, engineConfiguration->wwEnableAdaptiveLearning,
              Sensor::get(SensorType::Clt).value_or(-999), 
              Sensor::getOrZero(SensorType::Rpm));
    
    if (m_adaptiveData.combinedTransientActive) {
        efiPrintf("WW_TRANSIENT: MAP_deriv=%.2f, TPS_deriv=%.2f, pos=%d, neg=%d",
                  m_adaptiveData.loadDerivative, m_adaptiveData.tpsDerivative,
                  m_adaptiveData.isPositiveTransient, m_adaptiveData.isNegativeTransient);
    }
    
    if (m_adaptiveData.collectingImmediate || m_adaptiveData.collectingProlonged) {
        efiPrintf("WW_COLLECTING: immediate=%d, prolonged=%d, duration=%.2f",
                  m_adaptiveData.collectingImmediate, m_adaptiveData.collectingProlonged,
                  m_adaptiveData.transientDuration);
    }
}

// Adicionar em applyCorrectionToTable() antes das aplicações:
efiPrintf("WW_CORRECTION: beta=%.3f, tau=%.3f, rpm=%.0f, map=%.1f",
          betaCorrection, tauCorrection, rpm, map);
'''
    
    print("📝 Adicione este código para debug:")
    print(debug_code)
    
    print("\n🔧 Para ativar logs:")
    print("1. Adicionar o código acima no wall_fuel.cpp")
    print("2. Recompilar firmware")
    print("3. Conectar console e observar logs 'WW_DEBUG', 'WW_TRANSIENT', 'WW_COLLECTING', 'WW_CORRECTION'")
    print("4. Fazer transientes (acelerar/desacelerar) e verificar se são detectados")

def main():
    """Função principal de debug."""
    print("🚗 DEBUG DO WALL WETTING ADAPTATIVO")
    print("Investigando por que as correções não estão acontecendo...")
    
    # Executa verificações
    config_ok = check_configuration_parameters()
    check_enable_flags()
    analyze_conditions_chain()
    check_potential_issues()
    suggest_debug_code()
    
    # Resumo final
    print_header("RESUMO E PRÓXIMOS PASSOS")
    
    if config_ok:
        print("✅ Configuração dos parâmetros Aquino: OK")
    else:
        print("❌ Configuração dos parâmetros Aquino: PROBLEMAS ENCONTRADOS")
    
    print("\n🔧 PRÓXIMOS PASSOS RECOMENDADOS:")
    print("1. Verificar se wwEnableAdaptiveLearning = true na configuração")
    print("2. Verificar se wwMinCoolantTemp está configurado adequadamente (60-80°C)")
    print("3. Verificar se thresholds Aquino não estão muito altos")
    print("4. Adicionar código de debug e observar logs durante transientes")
    print("5. Verificar se sensores TPS, MAP e Lambda estão funcionando")
    print("6. Verificar se tabelas de correção estão inicializadas com 1.0")
    
    print("\n📋 VALORES RECOMENDADOS PARA TESTE:")
    print("wwAquinoAccelThresh = 30.0    # %/s")
    print("wwAquinoMapAccelThresh = 20.0 # kPa/s") 
    print("wwAquinoBetaAccelGain = 0.1   # ganho moderado")
    print("wwAquinoTauSlowGain = 0.1     # ganho moderado")
    print("wwAquinoMinLambdaErr = 0.05   # erro mínimo 5%")

if __name__ == "__main__":
    main() 