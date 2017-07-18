debug  = True
if debug:
    from boa_simulate import *
lib=boaimport('bb')

def say():
    lib.say()
    print 'this is from c'

say()

print 'chain uagage'
lib.lib.say()

    
