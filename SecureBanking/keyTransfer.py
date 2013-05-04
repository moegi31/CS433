#RSA public key transfer

# Libraries
from Crypto.PublicKey import RSA
from Crypto import Random
import Crypto


# Import keys
f = open('keyAPrivate.prv')
privateA = RSA.importKey(f.read())
f.close()

f = open('keyAPublic.pub')
publicA = RSA.importKey(f.read())
f.close()

f = open('keyBPrivate.prv')
privateB = RSA.importKey(f.read())
f.close()

f = open('keyBPublic.pub')
publicB = RSA.importKey(f.read())
f.close()

# A
# A will generate nonce1 and encrypt it with publicB
nonce1 = 'helloworld'
enc_nonce1 = publicB.encrypt(nonce1, None)

# B
# B receives enc_nonce1 from A.
# B decrypts with his private key
dec_nonce1 = privateB.decrypt(enc_nonce1)

# B then encrypts dec_nonce1 with A's public key and generates nonce2
# also encrpyted with A's public key 
nonce2 = 'ohhi'
ver_nonce1 = publicA.encrypt(dec_nonce1, None)
enc_nonce2 = publicA.encrypt(nonce2, None)

# A
# First A will confirm the nonce1 from B
dec_ver_nonce1 = privateA.decrypt(ver_nonce1)

if dec_ver_nonce1 == nonce1:
    print "nonce1 was correct!"
else:
    print "nonce1 was wrong!"

# Then it will decrypt nonce2 and encrpyt it with B's public
dec_nonce2 = privateA.decrypt(enc_nonce2)
ver_nonce2 = publicB.encrypt(dec_nonce2, None)

# B
# First B will confirm the nonce2 from A
dec_ver_nonce2 = privateB.decrypt(ver_nonce2)

if dec_ver_nonce2 == nonce2:
    print "nonce2 was correct!"
else:
    print "nonce2 was wrong!"


# A
# A will send a session key that is encrypted with his private and the public of B    
session_key = '123'

enc_session_key = publicB.encrypt(session_key, None)
signed_session_key = privateA.sign(session_key, None)

# B
# Retrieve session_key and verifiy the signature matches
dec_session_key = privateB.decrypt(enc_session_key)
if publicA.verify(dec_session_key, signed_session_key):
    print "Session key established!"

# Contine trasmission with the session_key


