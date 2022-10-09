import datetime
from pynput import keyboard

keyboard_data = []

def on_press(key):
    try:
        print('alphanumeric key {0} pressed'.format(
            key.char))
    except AttributeError:
        print('special key {0} pressed'.format(
            key))
    keyboard_data.append({
        'type': 'keyboard',
        'time': datetime.datetime.now(),
    })

def on_release(key):
    print('{0} released'.format(
        key))
    if key == keyboard.Key.esc:
        # Stop listener
        return False

# # Collect events until released
# with keyboard.Listener(
#         on_press=on_press,
#         on_release=on_release) as listener:
#     listener.join()

# ...or, in a non-blocking fashion:
# listener = keyboard.Listener(
#     on_press=on_press,
#     on_release=on_release)
# listener.start()


class KeyboardRecord:
    def __init__(self) -> None:
        self.listener = keyboard.Listener(
            on_press=on_press,
            on_release=on_release,
        )

    def start(self):
        self.listener.start()

    def join(self):
        self.listener.join()

    def stop(self):
        self.listener.stop()
