#!/usr/bin/env bash

# Use: ./sshpass-retry.sh $PASS command...
# Example: ./sshpass-retry.sh $PASS sftp -o StrictHostKeyChecking=no ${USER}@${HOST} <<SSHCMD
# mkdir ${DESTINATION_FOLDER}
# SSHCMD

PASS=$1
shift

max_retries=5
retry_count=0
retVal=1

while [ $retry_count -lt $max_retries ] && [ $retVal -ne 0 ]; do
  retry_count=$((retry_count + 1))
  if [ $retry_count -gt 1 ]; then
    echo "Attempt $retry_count..."
  fi

  # Forward the password via sshpass and execute the command
  sshpass -p "$PASS" "$@"
  retVal=$?

  if [ $retVal -ne 0 ]; then
    echo "Attempt $retry_count failed. Retrying in 10 seconds..."
    sleep 10
  fi
done

if [ $retVal -ne 0 ]; then
  echo "Command failed after $max_retries attempts"
  exit $retVal
fi

exit 0
