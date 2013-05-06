# Used for generating and saving new keys

# Libraries
from Crypto.PublicKey import RSA
from Crypto import Random
import Crypto

# Random number generator
random_generator = Crypto.Random.new().read

# Generate keys
bank = RSA.generate(1024, random_generator)
atm1 = RSA.generate(1024, random_generator)
atm2 = RSA.generate(1024, random_generator)

# Save these keys
f = open('bankKeys/bank.prv','w')
f.write(bank.exportKey())
f.close()

f = open('atmKeys/bank.pub','w')
f.write(bank.publickey().exportKey())
f.close()

f = open('atmKeys/atm1.prv','w')
f.write(atm1.exportKey())
f.close()

f = open('bankKeys/atm1.pub','w')
f.write(atm1.publickey().exportKey())
f.close()

f = open('atmKeys/atm2.prv','w')
f.write(atm2.exportKey())
f.close()

f = open('bankKeys/atm2.pub','w')
f.write(atm2.publickey().exportKey())
f.close()

