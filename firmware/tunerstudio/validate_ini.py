import sys
import re

def check_unbalanced_quotes(file_path):
    errors = 0
    with open(file_path, 'r', encoding='utf-8') as f:
        for line_num, line in enumerate(f, 1):
            line = line.strip()
            # Ignore comments
            if line.startswith(';') or line.startswith('#'):
                continue
            
            # Count double quotes
            quote_count = line.count('"')
            if quote_count % 2 != 0:
                print(f"Error: Unbalanced quotes at {file_path}:{line_num}")
                print(f"  Line: {line}")
                errors += 1
                
    return errors

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python validate_ini.py <file_path>")
        sys.exit(1)
        
    total_errors = 0
    for file_path in sys.argv[1:]:
        total_errors += check_unbalanced_quotes(file_path)
        
    if total_errors > 0:
        print(f"Total errors found: {total_errors}")
        sys.exit(1)
    else:
        print("No syntax errors found.")
        sys.exit(0)
