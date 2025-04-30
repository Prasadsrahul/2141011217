#!/bin/bash

# Assignment 1: Text Processing and Automation
# Description: Extracts all login attempts from /var/log/auth.log,
#              identifies the user and the error message,
#              and writes the results to a separate file.

# Output file
OUTPUT_FILE="login_attempts_report.txt"

# Clear the output file if it already exists
> "$OUTPUT_FILE"

# Check if the log file exists
LOG_FILE="/var/log/auth.log"
if [[ ! -f "$LOG_FILE" ]]; then
    echo "Error: $LOG_FILE not found. Are you running this on a Linux system with auth.log?"
    exit 1
fi

# Process failed login attempts
echo "Failed Login Attempts:" >> "$OUTPUT_FILE"
grep "Failed password" "$LOG_FILE" | awk '
{
    for(i=1;i<=NF;i++) {
        if($i == "for") user=$(i+1)
        if($i == "from") ip=$(i+1)
    }
    print "User: " user ", IP: " ip
}' >> "$OUTPUT_FILE"

# Separate section for invalid users (optional enhancement)
echo -e "\nInvalid User Attempts:" >> "$OUTPUT_FILE"
grep "Invalid user" "$LOG_FILE" | sed -E 's/.*Invalid user ([^ ]+) from ([^ ]+).*/User: \1, IP: \2/' >> "$OUTPUT_FILE"

echo -e "\n[✔] Report saved to $OUTPUT_FILE"

# Assignment 1: Text Processing and Automation
# Description: Extracts all login attempts from /var/log/auth.log,
#              identifies the user and the error message,
#              and writes the results to a separate file.

# Output file
OUTPUT_FILE="login_attempts_report.txt"

# Clear the output file if it already exists
> "$OUTPUT_FILE"

# Check if the log file exists
LOG_FILE="/var/log/auth.log"
if [[ ! -f "$LOG_FILE" ]]; then
    echo "Error: $LOG_FILE not found. Are you running this on a Linux system with auth.log?"
    exit 1
fi

# Process failed login attempts
echo "Failed Login Attempts:" >> "$OUTPUT_FILE"
grep "Failed password" "$LOG_FILE" | awk '
{
    for(i=1;i<=NF;i++) {
        if($i == "for") user=$(i+1)
        if($i == "from") ip=$(i+1)
    }
    print "User: " user ", IP: " ip
}' >> "$OUTPUT_FILE"

# Separate section for invalid users (optional enhancement)
echo -e "\nInvalid User Attempts:" >> "$OUTPUT_FILE"
grep "Invalid user" "$LOG_FILE" | sed -E 's/.*Invalid user ([^ ]+) from ([^ ]+).*/User: \1, IP: \2/' >> "$OUTPUT_FILE"

echo -e "\n[✔] Report saved to $OUTPUT_FILE"
#!/bin/bash

# Assignment 1: Text Processing and Automation
# Description: Extracts all login attempts from /var/log/auth.log,
#              identifies the user and the error message,
#              and writes the results to a separate file.

# Output file
OUTPUT_FILE="login_attempts_report.txt"

# Clear the output file if it already exists
> "$OUTPUT_FILE"

# Check if the log file exists
LOG_FILE="/var/log/auth.log"
if [[ ! -f "$LOG_FILE" ]]; then
    echo "Error: $LOG_FILE not found. Are you running this on a Linux system with auth.log?"
    exit 1
fi

# Process failed login attempts
echo "Failed Login Attempts:" >> "$OUTPUT_FILE"
grep "Failed password" "$LOG_FILE" | awk '
{
    for(i=1;i<=NF;i++) {
        if($i == "for") user=$(i+1)
        if($i == "from") ip=$(i+1)
    }
    print "User: " user ", IP: " ip
}' >> "$OUTPUT_FILE"

# Separate section for invalid users (optional enhancement)
echo -e "\nInvalid User Attempts:" >> "$OUTPUT_FILE"
grep "Invalid user" "$LOG_FILE" | sed -E 's/.*Invalid user ([^ ]+) from ([^ ]+).*/User: \1, IP: \2/' >> "$OUTPUT_FILE"

echo -e "\n[✔] Report saved to $OUTPUT_FILE"

