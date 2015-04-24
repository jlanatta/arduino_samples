#include <irdebug.h>
#include <apple-remote.h>
#define SIGNAL_MAX_LENGTH 70

void setup()
{
  Serial.begin(9600);
  Serial.println("Point your Apple Controller to your IR sensor.");
}

void loop()
{
  unsigned int signal[SIGNAL_MAX_LENGTH];
  bool captured;
  int code;

  captured = irdebug_capture_signal(signal, SIGNAL_MAX_LENGTH);

  if (captured) {
    irdebug_print_signal(signal);
    code = apple_remote_match(signal);

    switch (code) {
      case APPLE_REMOTE_MENU:
        Serial.println("[menu]");
        break;
      case APPLE_REMOTE_RIGHT:
        Serial.println("[right]");
        break;
      case APPLE_REMOTE_LEFT:
        Serial.println("[left]");
        break;
      case APPLE_REMOTE_UP:
        Serial.println("[up]");
        break;
      case APPLE_REMOTE_DOWN:
        Serial.println("[down]");
        break;
      case APPLE_REMOTE_PLAY:
        Serial.println("[play]");
        break;
      case APPLE_REMOTE_CENTER:
        Serial.println("[center]");
        break;
      case APPLE_REMOTE_REPEAT:
        Serial.println("[repeat]");
        break;
      default:
        Serial.println("[?]");
    }
  }
}

