import datetime
from pynput import keyboard

keyboard_data = []

def on_press(key):
    key_str = ''
    try:
        print('alphanumeric key {0} pressed'.format(
            key.char))
        key_str = key.char
    except AttributeError:
        print('special key {0} pressed'.format(
            key))
        key_str = str(key)

    keyboard_data.append({
        'type': 'keyboard',
        'time': datetime.datetime.now(),
        'key': key_str,
        'status': 'press',
    })

def on_release(key):
    key_str = ''
    print('{0} released'.format(
        key))
    if isinstance(key, keyboard.Key):
        key_str = str(key)
    elif isinstance(key, keyboard.KeyCode):
        key_str = key.char
    
    if key == keyboard.Key.esc:
        # Stop listener
        return False
    keyboard_data.append({
        'type': 'keyboard',
        'time': datetime.datetime.now(),
        'key': key_str,
        'status': 'release',
    })

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
