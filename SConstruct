#!python
import os
import sys
import scons_compiledb

VariantDir('obj', 'src', duplicate=0)
opts = Variables([], ARGUMENTS)

# Gets the standard flags CC, CCX, etc.
env = DefaultEnvironment()
env.VariantDir('obj', 'src', duplicate=0)

scons_compiledb.enable(env)

default_binding_dir = "../3rd-party/godot-cpp"

# Define our options
opts.Add(EnumVariable('target', "Compilation target", 'debug', ['d', 'debug', 'r', 'release']))
opts.Add(EnumVariable('platform', "Compilation platform", '', ['', 'windows', 'x11', 'linux', 'osx']))
opts.Add(EnumVariable('p', "Compilation target, alias for 'platform'", '', ['', 'windows', 'x11', 'linux', 'osx']))
opts.Add(BoolVariable('use_llvm', "Use the LLVM / Clang compiler", 'no'))
opts.Add(PathVariable('target_path', 'The path where the lib is installed.', 'bin'))
opts.Add(PathVariable('target_name', 'The library name.', 'libdodgethecreeps', PathVariable.PathAccept))
opts.Add(PathVariable('binding_dir', 'Path to Godot C++ bindings.', default_binding_dir))
opts.Add(PathVariable('headers_dir', 'Path to Godot headers.', '.'))
opts.Add(PathVariable('godot_library', 'Godot library name.', 'libgodot-cpp', PathVariable.PathAccept))

# only support 64 at this time..
bits = 64

# Updates the environment with the option variables.
opts.Update(env)

# Local dependency paths, adapt them to your setup
cpp_bindings_path = env['binding_dir']
godot_headers_path = env['headers_dir']
if godot_headers_path == '.':
    godot_headers_path = cpp_bindings_path + '/godot_headers'
cpp_library = env['godot_library']

# Process some arguments
if env['p'] != '':
    env['platform'] = env['p']

if env['use_llvm']:
    if env['platform'] == 'windows':
        env['CC'] = 'clang-cl'
        env['CXX'] = 'clang-cl'
    else:
        env['CC'] = 'clang'
        env['CXX'] = 'clang++'

if env['platform'] == '':
    if sys.platform.startswith('linux'):
        env['platform'] = 'linux'
    elif sys.platform == 'darwin':
        env['platform'] = 'osx'
    elif sys.platform == 'win32' or sys.platform == 'msys':
        env['platform'] = 'windows'
    else:
        print("No valid target platform selected.")
        quit();

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# Check our platform specifics
if env['platform'] == "osx":
    env['target_path'] += '/osx'
    cpp_library += '.osx'
    env.Append(CCFLAGS=['-arch', 'x86_64'])
    env.Append(CXXFLAGS=['-std=c++17'])
    env.Append(LINKFLAGS=['-arch', 'x86_64'])
    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS=['-g', '-O2'])
    else:
        env.Append(CCFLAGS=['-g', '-O3'])

elif env['platform'] in ('x11', 'linux'):
    env['target_path'] += '/linux'
    cpp_library += '.linux'
    env.Append(CCFLAGS=['-fPIC'])
    env.Append(CXXFLAGS=['-std=c++17'])
    if env['target'] in ('debug', 'd'):
        env.Append(CCFLAGS=['-g3', '-Og'])
    else:
        env.Append(CCFLAGS=['-g', '-O3'])

elif env['platform'] == "windows":
    env['target_path'] += '/win64'
    cpp_library += '.windows'
    # This makes sure to keep the session environment variables on windows,
    # that way you can run scons in a vs 2017 prompt and it will find all the required tools
    env.Append(ENV=os.environ)

    env.Append(CPPDEFINES=['WIN32', '_WIN32', '_WINDOWS', '_CRT_SECURE_NO_WARNINGS'])
    env.Append(CCFLAGS=['-W3', '-GR'])
    if env['target'] in ('debug', 'd'):
        env.Append(CPPDEFINES=['_DEBUG'])
        env.Append(CCFLAGS=['-EHsc', '-MDd', '-ZI'])
        env.Append(LINKFLAGS=['-DEBUG'])
    else:
        env.Append(CPPDEFINES=['NDEBUG'])
        env.Append(CCFLAGS=['-O2', '-EHsc', '-MD'])

if env['target'] in ('debug', 'd'):
    cpp_library += '.debug'
else:
    cpp_library += '.release'

cpp_library += '.' + str(bits)

# make sure our binding library is properly includes
env.Append(CPPPATH=['.', godot_headers_path, cpp_bindings_path + '/include/', cpp_bindings_path + '/include/core/', cpp_bindings_path + '/include/gen/'])
env.Append(LIBPATH=[cpp_bindings_path + '/bin/'])
env.Append(LIBS=[cpp_library])

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=['src/'])
sources = Glob('src/*.cpp')

library = env.SharedLibrary(target=env['target_path'] + '/' + env['target_name'] , source=sources)

Default(library)

# Generates help for the -h scons option.
Help(opts.GenerateHelpText(env))

env.CompileDb()
