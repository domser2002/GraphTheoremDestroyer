import pytest
import pyautogui
import Xlib
import time
import cv2
import numpy as np

def look_for_image(img_name: str, confidence_threshold=0.8, width_point=0.5):
    screenshot = pyautogui.screenshot()
    screen = np.array(screenshot)
    screen = cv2.cvtColor(screen, cv2.COLOR_RGB2BGR)

    img = cv2.imread(img_name, cv2.IMREAD_COLOR)

    screen_gray = cv2.cvtColor(screen, cv2.COLOR_BGR2GRAY)
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    result = cv2.matchTemplate(screen_gray, img_gray, cv2.TM_CCOEFF_NORMED)
    _, max_val, _, max_loc = cv2.minMaxLoc(result)

    if max_val >= confidence_threshold:
        img_height, img_width = img_gray.shape
        result_pos = (max_loc[0] + int(img_width * width_point), max_loc[1] + img_height // 2)
        return result_pos
    else:
        return None


def click_image(img_name: str, confidence_threshold=0.8, width_point=0.5):
    pos = look_for_image(img_name, confidence_threshold, width_point)
    if pos is not None:
        pyautogui.click(pos[0], pos[1])


def test_button_click():
    time.sleep(15)

    click_image('add_new_restriction.png')
    time.sleep(1)

    click_image('choose_restriction.png', width_point=0.9)
    time.sleep(1)

    click_image('has_no_cycles.png')
    time.sleep(1)

    click_image('add_new_restriction.png')
    time.sleep(1)

    click_image('choose_restriction.png', width_point=0.9)
    time.sleep(1)

    click_image('is_a_cycle.png')
    time.sleep(1)

    click_image('run.png')
    time.sleep(3)

    result_1_pos = look_for_image('expected_result_1.png', confidence_threshold=0.95)
    assert result_1_pos is not None