package com.opensr5.ini;

import java.util.Objects;

public class VeAnalyzeMap {
    private final String tableName;
    private final String lambdaTargetTableName;
    private final String lambdaChannel;
    private final String egoCorrectionChannel;
    private final String activeCondition;

    public VeAnalyzeMap(String tableName, String lambdaTargetTableName, String lambdaChannel, String egoCorrectionChannel, String activeCondition) {
        this.tableName = tableName;
        this.lambdaTargetTableName = lambdaTargetTableName;
        this.lambdaChannel = lambdaChannel;
        this.egoCorrectionChannel = egoCorrectionChannel;
        this.activeCondition = activeCondition;
    }

    public String getTableName() {
        return tableName;
    }

    public String getLambdaTargetTableName() {
        return lambdaTargetTableName;
    }

    public String getLambdaChannel() {
        return lambdaChannel;
    }

    public String getEgoCorrectionChannel() {
        return egoCorrectionChannel;
    }

    public String getActiveCondition() {
        return activeCondition;
    }

    @Override
    public String toString() {
        return "VeAnalyzeMap{" +
                "tableName='" + tableName + '\'' +
                ", lambdaTargetTableName='" + lambdaTargetTableName + '\'' +
                ", lambdaChannel='" + lambdaChannel + '\'' +
                ", egoCorrectionChannel='" + egoCorrectionChannel + '\'' +
                ", activeCondition='" + activeCondition + '\'' +
                '}';
    }
}
