from chatterbot import ChatBot
from chatterbot.trainers import ListTrainer
import os
bot=ChatBot('Bot')
while True:
    message=input('you:')
    if message.strip()!='bye':
    
        reply=bot.get_response(message)
        print('ChatBot :',reply)
    if message.strip()=='bye':
        print('chatbot : bye ')
        break