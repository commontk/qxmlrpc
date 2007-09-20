# simple test program
from xmlrpclib import ServerProxy, Error

server = ServerProxy("http://localhost:7777") # local server

try:
    print "sum(3,4):", server.sum(3,4)
    print "difference(3,4):", server.difference(3,4)
except Error, v:
    print "ERROR", v

print "next 2 calls should return errors"

try:
    print "sum2(3,4):", server.sum2(3,4)
except Error, v:
    print "ERROR", v

try:
    print "sum(3,'4'):", server.sum(3,'4')
except Error, v:
    print "ERROR", v

print 'methods:', server.system.listMethods()
print 'sum() method signature:', server.system.methodSignature('sum')
print 'sum() help string:', server.system.methodHelp('sum')
