ip=192.168.50.142:2001
#ip=192.168.1.1:2001
arduinoPort="/dev/ttyACM0"

linkTo="$HOME/dev/ttyACM0"

if [ -w $arduinoPort ]; then
    echo "linking $arduinoPort to $linkTo"
    socat -d -d "pty,link=$linkTo,raw,echo=0,waitslave" "file:$arduinoPort,nonblock,waitlock=/tmp/arduino.lock"
else
    echo "linking tcp:$ip to $linkTo"
    socat -d -d "pty,link=$linkTo,raw,echo=0,waitslave" "tcp:$ip,nonblock,waitlock=/tmp/arduino.lock"
fi
