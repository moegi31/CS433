# Used for generating and saving new keys

# Libraries
from Crypto.PublicKey import RSA
from Crypto import Random
import Crypto

# Random number generator
random_generator = Crypto.Random.new().read

# Generate two keys
keyA = RSA.generate(1024, random_generator)
keyB = RSA.generate(1024, random_generator)

# Save these keys
f = open('keyAPrivate.prv','w')
f.write(keyA.exportKey())
f.close()

f = open('keyAPublic.pub','w')
f.write(keyA.publickey().exportKey())
f.close()

f = open('keyBPrivate.prv','w')
f.write(keyB.exportKey())
f.close()

f = open('keyBPublic.pub','w')
f.write(keyB.publickey().exportKey())
f.close()
