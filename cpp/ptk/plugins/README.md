# PTK::PLUGINS 

- Allows dynamic loading of libraries and loading of class instances at runtime.

- Build Library (.so) and load a instance like 

```
PluginLoader dlloader("path/to/plugin");


PluginPtr special = dlloader.load("ClassName");
```
Classes must be registered as plugins using
```
REGISTER_PLUGIN_CLASS(DerivedType, PluginType)
```
The PluginType is the base class of Derived and must inherit from PluginBase. The macro must be called in the .cpp file (if multiple plugin classes in a single lib)

- TODO: finish writing CMakeLists function to build a pluginLin
- currently dlloader finds the plugin lib by modifying the string like: "lib" + plugin + ".so". Need to update this to check the input string type (or make extra class)
- smarter way of checking/searching for plugins. CmakeList function could install them somewhere?
- ability of ptklaunch (general args) to specify plugins/classes to inject so we can change things from config files