from chatterbot import ChatBot
from chatterbot.trainers import ListTrainer
import os
bot=ChatBot('Bot')
bot.set_trainer(ListTrainer)
for files in os.listdir('/home/prachi/PycharmProjects/untitled1/venv/lib/python3.6/chatterbot-corpus-master/chatterbot_corpus/data/english'):
	data=open('/home/prachi/PycharmProjects/untitled1/venv/lib/python3.6/chatterbot-corpus-master/chatterbot_corpus/data/english/' +files , 'r').readlines()
	bot.train(data)

while True:
	message=input('you:')
	if message.strip()!='Bye':
	
		reply=bot.get_response(message)
		print('ChatBot :',reply)
	if message.strip()=='Bye':
		print('chatbot : bye')
		break
