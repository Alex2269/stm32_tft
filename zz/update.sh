#!/bin/sh

   #******************************************
   # echo "# stm32_tft" >> README.md
   git init
   git status
   git add .
   git commit -m "$(date "+%Y-%m-%d")"
   git branch -M master
   git remote add origin git@github.com:Alex2269/stm32_tft.git
   git push -u origin master
   git pull
   #******************************************

   #******************************************
   git status
   git add .
   git commit -m "$(date "+%Y-%m-%d")"
   git push -u origin master
   git pull
   #******************************************

