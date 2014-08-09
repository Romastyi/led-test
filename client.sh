#!/bin/bash

help ()
{
echo "Send commands to LED server"
echo
echo "Usage:"
echo "  $0 COMMAND [VALUE]"
echo
echo "Commands:"
echo " -h, --help  : Show this information"
echo " -s, --state : Get\\set LED state"
echo " -c, --color : Get\\set LED color"
echo " -r, --rate  : Get\\set LED rate"
echo
echo "Note: If VALUE is specified then SET command will be sent, otherwise GET command will be sent."
echo
}

send_command ()
{
fifo_in="/tmp/led_pipe_in"
fifo_out="/tmp/led_pipe_out"

# Отправляем запрос
echo ${*:1} > $fifo_in
# Читаем ответ
tail < $fifo_out
}

# Обработка параметров
case "$1" in
  "-h" | "--help" )
  help
  ;;
  
  "-s" | "--state" )
  if [ -z "$2" ]
  then
    send_command "get-led-state"
  else
    send_command "set-led-state" "$2"
  fi
  ;;
  
  "-c" | "--color" )
  if [ -z "$2" ]
  then
    send_command "get-led-color"
  else
    send_command "set-led-color" "$2"
  fi
  ;;
  
  "-r" | "--rate" )
  if [ -z "$2" ]
  then
    send_command "get-led-rate"
  else
    send_command "set-led-rate" "$2"
  fi
  ;;
  
  # Оставим для завершения работы сервера.
  "exit" )
    send_command "exit"
  ;;
  
  *)
    echo "Illegal argumets"
    help
  ;;
esac
