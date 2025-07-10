# Configuration

- - - -

# Compilation options

ofxImGui comes with quite a few compilation options which define how imgui is interfaced with OpenFrameworks.
By setting them explicitly, you can enforce a specific way to meet your needs.  

An easy way to get started is to call the configuration assistant (see below) and interactively explore your options.

## Backends 

All ImGui APIs need to define a **platform and renderer backend**. The platform backend handles window initialisation and interactions with ImGui. The renderer backend handles rendering to the created surface(s). 

### GL context / Renderer backend

ofxImGui uses the native ImGui renderer backend. slightly modified version of the default 
DearImGui needs to know your GL Context. ofxImGui tries to match your project's settings.  
If your projects needs to force a specific GL configuration, you can set some native imgui compilation flags to match your project settings :
 - `IMGUI_IMPL_OPENGL_ES2` --> Use GLES2 (or GL ES 1.1 with some hacks).
 - `IMGUI_IMPL_OPENGL_ES3` --> Use GLES3.
 - `[none of the previous ones]` --> Use OpenGL.

### Platform Backend

By default, ofxImGui uses the official ImGui GLFW backend when using `ofAppGLFWWindow`, this ensures full compatibility with any ImGui feature, and is interfaced directly with GLFW. The OpenFrameworks engine is customly interfaced to OF and implements minimal ImGui support. [More info](./PlatformSupport.md#backend-support-table).

Most supported OpenFrameworks build targets rely on GLFW (personal computer targets), with the exception of mobile targets (which use their respective platform windowing library), or linux builds without desktop environments (GLUT windows).

You may force to use the openframeworks backend by defining :
 - `OFXIMGUI_FORCE_OF_BACKEND`

#### GLFW backend options

You may also override some automatic macro defines (not recommended, there are drawbacks, but it might solve some very specific use cases):

- `IMGUI_GLFW_INJECT_MULTICONTEXT_SUPPORT=0` to disable imgui_impl_glfw changes to support multiple context.  
  Disables using ofxImGui within multiple `ofAppBaseWindow`s.
- `INTERCEPT_GLFW_CALLBACKS=0` to use an alternative method to bind imgui to glfw events.  
   - If `0`, ofxImGui doesn't add multi-context event routing, disabling multi-window-ofApp support.  
     Event propagation: `GLFW -> ImGui -> OpenFrameworks`.
   - If `1` (default), ofxImGui binds to GLFW, allowing to route events to the correct context instances.  
     Event propagation: `GLFW -> ofxImGui -> (ImGui + OpenFrameworks)`.

### Configuration assistant

To have an insight on how your ofxImGui interfaces ImGui, you can call `gui.drawOfxImGuiDebugWindow();` together with `OFXIMGUI_DEBUG`. It contains an assistant that provides an explanation of your current configuration. It also provides some suggestions for gradually improving your configuration (to get the most out of OF+ImGui).

- - - -

# Update GLFW 

If you use the GLFW backend (enabled by default), the [GLFW version that ships with oF or your distro](./PlatformSupport.md#Glfw-version) is probably not too recent. In order to enable more native mouse cursors, and possibly other interface polishings, you can update GLFW within your oF installation.

Only GLFW 3.4+ supports all ImGui features. Versions below gradually disable some small UI features making ImGui slightly less pretty.  
**Follow this step only if you have OF < v_0.12.1 which already ships with GLFW 3.4.**

Also note that GLFW 3.4 brings better window-resizing (no black window while resizing) to all your ofApps !

#### On Windows and MacOs

**Warning**: BigSur and above used to break glfw-updates, probably the reason why OF didn't ship with more recent versions. Proceed with caution !
````bash
# Instructions for Mac/Win
cd OF/scripts
# Only if you don't have apothecary (OF release zip):
git clone https://github.com/openframeworks/apothecary.git
# Manually edit the GLFW formula `apothecary/apothecary/apothecary/formulas/glfw.sh`, change to :
# - `VER=3.3-stable` (for gfwf 3.3.x)
# - `VER=master` (for gfwf 3.4.x, recommended)
# - `GIT_URL=https://github.com/glfw/glfw.git`
# Manually delete `apothecary/apothecary/build/glfw if it exists
# Update (change osx by vs/msys2/linux/linux64/linuxarmv6l/linuxarmv7l or remove `-t osx` for autoselect)
./apothecary/apothecary/apothecary -t osx -j 4 update glfw
# Copy ./apothecary/glfw to OF/libs/
./apothecary/apothecary/apothecary -t osx -j 4 copy glfw
# Recompile oF (github installs only, not releases)
# cd ./osx && ./compileOF.sh -j3
````  

#### On Linux

````bash
# Instructions for rpi distros and Linux desktops
# Raspbian <= Buster don't have GLFW 3.3 in their repos, but you can try.
apt update && apt upgrade libglfw3 libglfw3-dev
# Show your current version
apt-cache show libglfw3-dev
````  
_This should also enable gamepad support on RPI with Raspbian <= Buster which ships with GLFW <= 3.2.1._  
_Raspbian Note : Packages are known to be a little outdated, the easiest way is to upgrade your distro to use the latest version. At the time of OF_v0.11.0, Raspbian shipped with GLFW 3.2 for example._

Note: On Linux, your app won't run if an older GLFW version is installed on the distro.

#### After the GLFW update

Define `OFXIMGUI_GLFW_NO_VERSION_HACKS` to tell ofxImGui to force-use more precise cursors & more.


- - - -

