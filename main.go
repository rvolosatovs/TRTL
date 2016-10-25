package main

import (
	"io"
	"os"
	"time"

	"github.com/apex/log"
	"github.com/apex/log/handlers/cli"
	evdev "github.com/gvalkov/golang-evdev"
	"github.com/hybridgroup/gobot"
	"github.com/spf13/viper"
	"github.com/tarm/serial"
)

var gbot = gobot.NewGobot()

func init() {
	log.SetHandler(cli.New(os.Stdout))
	log.SetLevel(log.DebugLevel)

	viper.SetDefault("baud", 115200)
	viper.SetDefault("serial-port", "/home/b1101/dev/ttyACM0")

	viper.SetDefault("grabber-pull-servo", "9")
	viper.SetDefault("grabber-arm-servo", "3")

	viper.SetDefault("camera-servo-X", "5")
	viper.SetDefault("camera-servo-Y", "6")

	viper.SetDefault("motor-right-enable", "10")
	viper.SetDefault("motor-right-direction-forward", "4")
	viper.SetDefault("motor-right-direction-backward ", "7")

	viper.SetDefault("motor-left-enable", "11")
	viper.SetDefault("motor-left-direction-forward", "12")
	viper.SetDefault("motor-left-direction-backward ", "13")

	viper.SetConfigName("trtl")
	viper.SetConfigType("yaml")
	viper.AddConfigPath(".")
	switch err := viper.ReadInConfig(); {
	case err != nil && !os.IsNotExist(err):
		log.WithError(err).Fatal("Error reading config")
	}
}

func main() {
	log.Info("opening connection")
	arduino, err := serial.OpenPort(&serial.Config{Name: viper.GetString("serial-port"), Baud: viper.GetInt("baud")})
	if err != nil {
		log.WithError(err).Fatal("cant connect to arduino")
	}
	trtl := &trtl{arduino: arduino}
	log.Info("connected")

	var js *evdev.InputDevice
	for range time.Tick(time.Second) {
		if js, err = evdev.Open("/dev/input/by-id/usb-Sony_PLAYSTATION_R_3_Controller-event-joystick"); err != nil {
			log.WithError(err).Error("failed to connect to joystick")
		} else {
			log.Info("joystick connected")
			break
		}
	}

	for {
		events, err := js.Read()
		if err != nil {
			log.WithError(err).Error("Failed to read joystick")
		}

		for _, event := range events {
			if event.Type == evdev.EV_KEY {
				key := evdev.NewKeyEvent(&event)

				if key.State != evdev.KeyDown {
					if key.State == evdev.KeyUp {
						trtl.stop()
					}
					continue
				}

				log.WithField("keycode", key.Scancode).Debug("")

				switch key.Scancode {

				// Camera

				case 300:
					trtl.look(up)
				case 301:
					trtl.look(right)
				case 302:
					trtl.look(down)
				case 303:
					trtl.look(left)

				// Drivetrain

				case 292:
					trtl.drive(up)
				case 293:
					trtl.drive(right)
				case 294:
					trtl.drive(down)
				case 295:
					trtl.drive(left)

				// Grabber

				case 296:
					trtl.graberDown()
				case 289:
					trtl.pull()
				case 290:
					trtl.grab()

				// Led

				case 299:
					trtl.ledOn()
				case 297:
					trtl.ledOff()

				default:
					trtl.ledOn()

					log.WithField("scancode", key.Scancode).Warn("unknown key")
				}
			}
		}
	}
}

type direction int8

const (
	up direction = iota
	down
	right
	left
)

type trtl struct {
	arduino *serial.Port
}

func send(w io.Writer, b byte) (err error) {
	_, err = w.Write([]byte{b})
	return
}

func mustSend(w io.Writer, b byte) {
	if err := send(w, b); err != nil {
		log.WithError(err).WithField("byte", b).Fatal("Failed to send byte")
	}
}

func (t *trtl) ledOn() {
	mustSend(t.arduino, 'c')
}

func (t *trtl) ledOff() {
	mustSend(t.arduino, 'x')
}

func (t *trtl) drive(d direction) {
	var b byte

	switch d {
	case left:
		b = 'h'

	case down:
		b = 'j'

	case up:
		b = 'k'

	case right:
		b = 'l'
	}

	mustSend(t.arduino, b)
}

func (t *trtl) look(d direction) {
	var b byte

	switch d {
	case left:
		b = 's'

	case up:
		b = 'd'

	case down:
		b = 'f'

	case right:
		b = 'g'
	}

	mustSend(t.arduino, b)
}
func (t *trtl) stop() {
	mustSend(t.arduino, 'n')
}

func (t *trtl) graberDown() {
	mustSend(t.arduino, 'r')
}
func (t *trtl) grab() {
	mustSend(t.arduino, 't')
}
func (t *trtl) pull() {
	mustSend(t.arduino, 'y')
}