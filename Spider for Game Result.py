from bs4 import BeautifulSoup
from bs4 import NavigableString
from urllib.request import urlopen
from selenium import webdriver
import re

driver = webdriver.Chrome()
driver.get("http://202.120.167.41:99/")
driver.find_element_by_link_text("游客").click()

list = []

print(1)


driver.get("http://202.120.167.41:99/submission/5c26ec027a114f76098a2bb3/3")
base_url = "http://202.120.167.41:99"
his = ["/submission/5c1b27c37a114f760928f8ac"]
while (True):
    page = 0
    url = base_url + his[-1]

    html = driver.page_source
    soup = BeautifulSoup(html, features = 'lxml')
    draw = soup.find_all('a',{"class":"match-status--text draw"})
    draw = [l['href'] for l in draw]
    counter = 0
    #print('draw')
    while (counter<len(draw)):
        url = base_url + draw[counter]
        html = urlopen(url).read().decode('utf-8')
        soup = BeautifulSoup(html, features='lxml')
        name = soup.find('th', {"class": "col--challenger round-status--cell"})
        if ( name.string.find("游客")<0 ):
            #print('draw',name.get_text())
            status = soup.find_all("span")
            for i in status:
                if (i.find("Win")==None and i.find("Lose")==None):
                    status.remove(i)
            if (status[2].string.find("Lose") >=0): print('draw',name.get_text(),"White")
            if (status[5].string.find("Lose") >=0): print('draw',name.get_text(),"Black")

        name = soup.find('th', {"class": "col--challengee round-status--cell"})
        if ( name.string.find("游客")<0 ):
            #print('draw',name.get_text())
            status = soup.find_all("span")
            for i in status:
                if (i.find("Win") == None and i.find("Lose") == None):
                    status.remove(i)
            if (status[4].string.find("Lose") >=0): print('draw', name.get_text(), "White")
            if (status[1].string.find("Lose") >=0): print('draw', name.get_text(), "Black")
        counter=counter+1



    html = driver.page_source
    soup = BeautifulSoup(html, features='lxml')
    lose = soup.find_all('a',{"class":"match-status--text lose"})
    lose = [l['href'] for l in lose]
    counter = 0
    #print('lose')
    while (counter<len(lose)):
        url = base_url + lose[counter]
        html = urlopen(url).read().decode('utf-8')
        soup = BeautifulSoup(html, features='lxml')
        name = soup.find('th', {"class": "col--challenger round-status--cell"})
        if ( name.string.find("游客")<0 ):
            #print('lose',name.get_text())
            status = soup.find_all("span")
            for i in status:
                if (i.find("Win") == None and i.find("Lose") == None):
                    status.remove(i)
            #print(status)
            if (status[2].string.find("Lose") >=0): print('lose', name.get_text(), "White")
            if (status[5].string.find("Lose") >=0): print('lose', name.get_text(), "Black")

        name = soup.find('th', {"class": "col--challengee round-status--cell"})
        if ( name.string.find("游客")<0 ):
            #print('lose',name.get_text())
            status = soup.find_all("span")
            for i in status:
                if (i.find("Win") == None and i.find("Lose") == None):
                    status.remove(i)
            #print(status)
            if (status[4].string.find("Lose") >=0): print('lose', name.get_text(), "White")
            if (status[1].string.find("Lose") >=0): print('lose', name.get_text(), "Black")
        counter=counter+1

    #print(soup.find('th', {"class": "col--challengee round-status--cell"}))
    driver.find_element_by_link_text("Next ›").click()
