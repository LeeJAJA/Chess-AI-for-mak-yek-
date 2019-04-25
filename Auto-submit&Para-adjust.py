from selenium import webdriver
from bs4 import BeautifulSoup
from urllib.request import urlopen
import time

x6 = 30
x7 = 350
count = 0

while count<4 :
    html = urlopen("http://202.120.167.41:99/submission/user/5bf1509e4d9a9e2201ca4a39").read().decode('utf-8')
    soup = BeautifulSoup(html, features='lxml')
    #print(soup)
    running = soup.find_all('td', {"class": "col--status submission-status--border running"})
    #print(running)

    if (running==[]) :
        count = count + 1
        driver = webdriver.Chrome()     # 打开 Chrome 浏览器
        driver.get("http://202.120.167.41:99/login")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").clear()
        driver.find_element_by_name("username").send_keys("1750655")
        driver.find_element_by_xpath("//body").click()
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").clear()
        driver.find_element_by_name("password").send_keys("")
        driver.find_element_by_xpath("//body").click()
        driver.find_element_by_xpath(
            "(.//*[normalize-space(text()) and normalize-space(.)='Password'])[1]/following::button[1]").click()
        driver.find_element_by_link_text("Submission").click()
        driver.find_element_by_link_text("Submit New Brain").click()
        driver.find_element_by_name("code").click()
        driver.find_element_by_name("code").clear()

        if (count == 1): path = '/usr/local/code/part1.c'
        if (count == 2): path = '/usr/local/code/part2.c'
        if (count == 3): path = '/usr/local/code/part3.c'
        openFile = open(path)
        code = openFile.read()
        driver.find_element_by_name("code").send_keys(code)
        driver.find_element_by_xpath("(.//*[normalize-space(text()) and normalize-space(.)='Settings'])[1]/following::button[1]").click()
        driver.close()

    time.sleep(300)



