from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()

src += Glob('sensor_icm20948.c')
src += Glob('driver/*.c')
if GetDepend('PKG_ICM20948_USING_SAMPLE'):
    src += Glob('example/*.c')

# add icm20948 include path.
path  = [cwd, cwd + '/driver']

# add src and include to group.
group = DefineGroup('icm20948', src, depend = ['PKG_USING_ICM20948'], CPPPATH = path)

Return('group')
