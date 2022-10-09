from pynput import mouse
import datetime

mouse_data = []

def on_move(x, y):
    print('Pointer moved to {0}'.format(
        (x, y)))
    mouse_data.append({
        "type": "mouse",
        "sub_type": "move",
        "time": datetime.datetime.now(),
        'x': x,
        'y': y,
    })
    

def on_click(x, y, button, pressed):
    print('{0} at {1}, button {2}'.format(
        'Pressed' if pressed else 'Released',
        (x, y), button))
    # if not pressed:
    #     # Stop listener
    #     return False
    mouse_data.append({
        'type': 'mouse',
        'sub_type': 'press' if pressed else 'release',
        'time': datetime.datetime.now(),
        'x': x,
        'y': y,
    })

def on_scroll(x, y, dx, dy):
    print('Scrolled {0} at {1}'.format(
        'down' if dy < 0 else 'up',
        (x, y)))
    mouse_data.append({
        'type': 'mouse',
        'sub_type': 'scroll',
        'time': datetime.datetime.now(),
        'x': x,
        'y': y,
    })

# Collect events until released
# with mouse.Listener(
#         on_move=on_move,
#         on_click=on_click,
#         on_scroll=on_scroll) as listener:
#     listener.join()

# # ...or, in a non-blocking fashion:
# listener = mouse.Listener(
#     on_move=on_move,
#     on_click=on_click,
#     on_scroll=on_scroll)
# listener.start()


class MouseRecord:
    def __init__(self) -> None:
        self.listener = mouse.Listener(
            on_move=on_move,
            on_click=on_click,
            on_scroll=on_scroll
        )

    def start(self):
        self.listener.start()

    def join(self):
        self.listener.join()

    def stop(self):
        self.listener.stop()