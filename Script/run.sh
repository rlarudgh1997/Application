#!/bin/bash

# Base paths and application configuration
BASE_DIR="$(dirname $(realpath $0))"
APP_PATH="$BASE_DIR/.."
BIN_NAME="Application"
# SDK_HOST=${CCOS_LIB_DIR:-/opt/sfc/CV}/environment-setup-sfc
# ALTON_PATH=${CCOS_LIB_DIR:-/opt/sfc/CV}/services
SDK_HOST=${CCOS_LIB_DIR}/environment-setup-sfc
ALTON_PATH=${CCOS_LIB_DIR}/services
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

# Check IP address (true : 0, false : 1)
getIPAddress() {
    local ip_address="$1"

    # 정규 표현식: IPv4 주소 확인
    if [[ $ip_address =~ ^([0-9]{1,3}\.){3}[0-9]{1,3}$ ]]; then
        # 각 옥텟 값이 0~255 범위인지 확인
        IFS='.' read -r o1 o2 o3 o4 <<< "$ip_address"
        if (( o1 >= 0 && o1 <= 255 && o2 >= 0 && o2 <= 255 && o3 >= 0 && o3 <= 255 && o4 >= 0 && o4 <= 255 )); then
        	# echo "유효한 IP 주소입니다: $ip_address"
            return 0  # true
        fi
    fi

	# echo "유효하지 않은 IP 주소입니다: $ip_address"
    return 1  # false
}

# Set environment variables based on platform
setEnvironments() {
    local env=$1

    case "$env" in
        target|t)
            echo "[Environment : Target]"
            APP_PATH="/home/root/App"
            ;;
        xserver|xs)
            local ip_address=$2
            if getIPAddress "$ip_address"; then
                export DISPLAY="$ip_address:0"
            else
                export DISPLAY=$(awk '/nameserver / {print $2; exit}' /etc/resolv.conf 2>/dev/null):0.0
            fi
            APP_PATH="$APP_PATH/deploy_x86"
            ;;
        host|h)
            local app_name=$2
            echo "[Environment : Host] - $app_name"
            if [ "$app_name" == "altonservice" ]; then
                source "$SDK_HOST" CV
                APP_PATH="$ALTON_PATH/$app_name"
                BIN_NAME="$app_name"
            elif [ "$app_name" == "Cluster" ]; then
                source "$SDK_HOST" PV
                APP_PATH="$APP_PATH/deploy_x86"
                BIN_NAME="$app_name"
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
    echo "    BIN_NAME  = $BIN_NAME"
    separator
}

# Run the process
runProcess() {
    local service=$1
    local app_path=$2
    local arguments=$3
    local full_path="$app_path/$service"

    local xserver=false
    local ip_address=$arguments
    if getIPAddress "$ip_address"; then
        xserver=true
    fi

    echo "[Process : Run]"
    echo "    INPUT_ARG = $1, $2, $3"
    echo "    SERVICE   = $service"
    echo "    FULL_PATH = $full_path"
    echo "    ARGUMENTS = $arguments"
    # echo "    ARGUMENTS = ${ARGUMENTS[*]}"

    if pgrep -x "$service" > /dev/null; then
        echo "$service is already running. Terminating it..."
        pkill -9 "$service"
    fi

    echo

    if [ -f "$full_path" ]; then
        if [ "$xserver" == "true" ]; then
            nohup "$full_path" > /dev/null 2>&1 &
        # elif [ "$background" == "true" ]; then
        #     # nohup "$full_path" > /dev/null 2>&1 &
        #     nohup "$full_path" s"${ARGUMENTS[@]}" > /dev/null 2>&1 &
        else
            # "$full_path" &
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

case "$PLATFORM" in
    target|t)
        setEnvironments target "$1"
        ARGUMENTS=("$@")
        killProcess "$BIN_NAME"
        runProcess "$BIN_NAME" "$APP_PATH" "$1"
        ;;
    xserver|xs)
        setEnvironments xserver "$1"
        ARGUMENTS=("$@")
        killProcess "$BIN_NAME"
        runProcess "$BIN_NAME" "$APP_PATH" "$1"
        ;;
    host|h)
        setEnvironments host "$1"
        ARGUMENTS=("$@")
        killProcess "$BIN_NAME"
        runProcess "$BIN_NAME" "$APP_PATH" "$1"
        ;;
    *)
        echo "Usage X-Server : $0 [xserver|xs] [10.45.143.15]"
        echo "Usage Target   : $0 [target|t]"
        echo "Usage Host     : $0 [host|h]"
        exit 1
        ;;
esac

displayDateTime

echo
echo
