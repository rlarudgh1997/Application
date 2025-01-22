#!/bin/bash

# Base paths and application configuration
BASE_DIR="$(dirname $(realpath $0))"
APP_PATH="$BASE_DIR/.."
BIN_NAME="Application"
SDK_HOST=${CCOS_LIB_DIR:-/opt/sfc/PV}/environment-setup-sfc
ARGUMENTS=()

clear
clear
clear

# Print separator for better output readability
separator() {
    echo "=========================================================================================================="
}

# Set file permissions
setPermissions() {
    chmod -R 755 "$BASE_DIR"
}

# Set environment variables based on platform
setEnvironments() {
    local env=$1
    local ip_address=$2

    case "$env" in
        target|t)
            echo "[Environment : Target]"
            APP_PATH="/home/root/App"
            ;;
        xserver|xs)
            echo "[Environment : Host XServer]"
            if [ -z "$ip_address" ]; then
                export DISPLAY=$(awk '/nameserver / {print $2; exit}' /etc/resolv.conf 2>/dev/null):0.0
            else
                export DISPLAY="$ip_address:0"
            fi
            APP_PATH="$APP_PATH/deploy_x86"
            ;;
        host|h)
            echo "[Environment : Host]"
            if [ "$BIN_NAME" == "altonservice" ]; then
                source "$SDK_HOST" CV
                APP_PATH="$CCOS_LIB_DIR/bin"
            elif [ "$BIN_NAME" == "Cluster" ]; then
                source "$SDK_HOST" PV
                APP_PATH="$APP_PATH/deploy_x86"
            else
                APP_PATH="$APP_PATH/deploy_x86"
            fi
            ;;
        *)
            echo "Invalid environment: $env"
            exit 1
            ;;
    esac

    echo "    SDK_HOST  = $SDK_HOST"
    echo "    APP_PATH  = $APP_PATH"
    separator
}

# Function to choose an option from a list with numbers
selectOptionWithNumbers() {
    local prompt=$1
    shift
    local options=("$@")

    echo "$prompt"
    for i in "${!options[@]}"; do
        echo "$((i + 1)): ${options[$i]}"
    done

    # Wait for valid input
    while true; do
        echo -n "Enter the number of your choice: "
        read choice

        if [[ "$choice" =~ ^[0-9]+$ ]] && ((choice >= 1 && choice <= ${#options[@]})); then
            echo "${options[$((choice - 1))]}"
            return
        else
            echo "Invalid choice, please enter a valid number between 1 and ${#options[@]}."
        fi
    done
}

# Handle user input for mode and module selection
getUserInput() {
    local service=$1
    local app_path=$2
    # local full_path="$app_path/$service"
    local full_path="/home/ivis/900_Code/610_Application/tc_creator/Script/../deploy_x86/Application"

    while true; do
        # Get input from the user
        echo "Enter mode (cv/pv) or type 'exit' to quit: "
        read -r mode
        if [ "$mode" == "exit" ]; then
            break
        fi

        # Check if the input is already in the arguments to avoid duplication
        if [[ ! " ${ARGUMENTS[@]} " =~ " ${mode} " ]]; then
            ARGUMENTS+=("$mode")
        fi

        # Run the app with the latest input
        echo "Running app with input: ${ARGUMENTS[*]}"
        "$full_path" "${ARGUMENTS[@]}"
    done
}

# Run the process
runProcess() {
    local service=$1
    local app_path=$2
    local background=$3
    local full_path="$app_path/$service"

    echo "[Process : Run]"
    echo "    SERVICE   = $service"
    echo "    FULL_PATH = $full_path"
    echo "    ARGUMENTS = ${ARGUMENTS[*]}"

    if pgrep -x "$service" > /dev/null; then
        echo "$service is already running. Terminating it..."
        pkill -9 "$service"
    fi

    echo

    if [ -f "$full_path" ]; then
        if [ "$background" == "true" ]; then
            nohup "$full_path" "${ARGUMENTS[@]}" > /dev/null 2>&1 &
        else
            "$full_path" "${ARGUMENTS[@]}" &
        fi

        sleep 0.1

        if pgrep -x "$service" > /dev/null; then
            echo "    [$service] is running."
        else
            echo "    [$service] is not running."
        fi
    else
        echo "    Executable not found: $full_path"
    fi

    separator
}

# Terminate all processes related to the binary
killProcess() {
    local service=$1

    if pkill -9 "$service" > /dev/null 2>&1; then
        echo "[Process : Kill]"
        echo "    Running processes found for [$service]."

        separator
    fi
}

# Display the current date and time
displayDateTime() {
    echo "[DateTime]"

    CURRENT_DATE_TIME=$(date +"%Y-%m-%d %H:%M:%S.%3N")
    echo "    $CURRENT_DATE_TIME"

    separator
}

# Main execution logic
separator

# Determine platform and environment
PLATFORM=$1

shift # Remove the first argument

# Environment setup based on platform
case "$PLATFORM" in
    target|t)
        setEnvironments target
        ARGUMENTS=("$@")
        killProcess "$BIN_NAME"
        runProcess "$BIN_NAME" "$APP_PATH"
        ;;
    xserver|xs)
        setEnvironments xserver "$1"
        ARGUMENTS=("$@")
        killProcess "$BIN_NAME"
        runProcess "$BIN_NAME" "$APP_PATH"
        ;;
    host|h)
        setEnvironments host
        ARGUMENTS=("$@")
        killProcess "$BIN_NAME"
        runProcess "$BIN_NAME" "$APP_PATH"
        ;;
    cli|c)
        setEnvironments target
        killProcess "$BIN_NAME"
        runProcess "$BIN_NAME" "$APP_PATH"
        getUserInput "$BIN_NAME" "$APP_PATH"
        ;;
    *)
        echo "Usage: $0 {host|h|target|t|xserver|xs} [optional arguments]"
        exit 1
        ;;
esac

displayDateTime

echo
echo
