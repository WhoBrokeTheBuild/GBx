
import gi
gi.require_version("Gtk", "3.0")
from gi.repository import GLib, Gtk, Gio, Gdk

import numpy
from ctypes import *
from OpenGL.GL import *

from . import Resource
from .libGBx import GBx
from .LogWindow import LogWindow
from .SerialWindow import SerialWindow

VERTEX_SHADER = '''
#version 330 core

in vec4 inPosition;
in vec2 inUV;

out vec2 vUV;

void main() {
    gl_Position = inPosition;
    vUV = inUV;
}
'''

FRAGMENT_SHADER = '''
#version 330 core

in vec2 vUV;

uniform sampler2D uTexture;
uniform vec2 uSize;
uniform vec2 uOffset;

out vec4 oColor;

void main() {
  vec2 uv = (vUV - uOffset) / uSize;
  if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
    oColor = vec4(0, 0, 0, 1);
  } else {
    oColor = texture2D(uTexture, uv);
  }
}
'''

# vec4 position, vec2 uv
VERTICES = numpy.array(
    [
        -1.0,  1.0,  0.0,  1.0,     0.0, 0.0,
         1.0,  1.0,  0.0,  1.0,     1.0, 0.0,
         1.0, -1.0,  0.0,  1.0,     1.0, 1.0,
         1.0, -1.0,  0.0,  1.0,     1.0, 1.0,
        -1.0, -1.0,  0.0,  1.0,     0.0, 1.0,
        -1.0,  1.0,  0.0,  1.0,     0.0, 0.0,
    ], 
    dtype='float32'
)

POSITION_SIZE = numpy.float32(0).nbytes * 4
UV_SIZE       = numpy.float32(0).nbytes * 2
VERTEX_SIZE   = POSITION_SIZE + UV_SIZE

@Gtk.Template(resource_path="/GBx/AppWindow.ui")
class AppWindow(Gtk.ApplicationWindow):
    __gtype_name__ = 'GBxAppWindow'

    gtkGLArea = Gtk.Template.Child()

    def __init__(self, scale=3):
        super(AppWindow, self).__init__()

        self.gtkGLArea.set_required_version(3, 3)

        self.isOpen = False
        self.isRunning = False
        self.isFullscreen = False

        self.logWindow = LogWindow()

        self.GBx = GBx()
        if not self.GBx.Init():
            print('Failed to initialize GBx')

        self.serialWindow = SerialWindow(self)

        self.set_scale(scale)
        self.set_position(Gtk.WindowPosition.CENTER)

        self.serialWindow.show()
        self.logWindow.show()

        
    @Gtk.Template.Callback('on_delete_event')
    def on_delete_event(self, obj, event):
        self.isOpen = False
        return True
        
    @Gtk.Template.Callback('on_destroy')
    def on_destroy(self, obj):
        self.serialWindow.destroy()
        
        self.GBx.Term()

        self.logWindow.destroy()

    def set_scale(self, scale):
        self.gtkGLArea.set_size_request(
            GBx.SCREEN_WIDTH * scale,
            GBx.SCREEN_HEIGHT * scale
        )

        # Scale back down to requested size
        self.resize(1, 1)

    @Gtk.Template.Callback('show_about')
    def show_about(self, obj):
        authors = [
            'Stephen L.W.',
            'Dillon B.',
            'Benjamin ',
        ]

        website = 'https://github.com/WhoBrokeTheBuild/GBx'

        about = Gtk.AboutDialog(
            program_name='GBx',
            version=self.GBx.GetVersionString(),
            authors=authors,
            website=website,
            website_label=website,
            license_type=Gtk.License.MIT_X11,
            logo_icon_name='application-x-executable'
        )

        about.show()

    @Gtk.Template.Callback('show_log_window')
    def show_log_window(self, obj):
        self.logWindow.show()

    @Gtk.Template.Callback('open_rom')
    def open_rom(self, obj):

        dialog = Gtk.FileChooserDialog(
            title='Open ROM',
            parent=self,
            action=Gtk.FileChooserAction.OPEN,
            buttons=(
                Gtk.STOCK_CANCEL, Gtk.ResponseType.REJECT,
                Gtk.STOCK_OPEN, Gtk.ResponseType.ACCEPT,
            )
        )

        filterROMs = Gtk.FileFilter.new()
        filterROMs.set_name('GB/GBC ROMs (*.gb, *.gbc)')
        filterROMs.add_pattern('*.gb')
        filterROMs.add_pattern('*.gbc')
        filterROMs.add_mime_type('application/x-gameboy-rom')
        dialog.add_filter(filterROMs)

        filterAll = Gtk.FileFilter.new()
        filterAll.set_name('All Files')
        filterAll.add_pattern('*')
        dialog.add_filter(filterAll)

        result = dialog.run()
        if result == Gtk.ResponseType.ACCEPT:
            dialog.hide()

            filename = dialog.get_filename()

            loaded = self.GBx.Cartridge_Load(filename.encode())
            if loaded:
                self.isRunning = True
                self.GBx.Reset()
            else:
                errorDialog = Gtk.MessageDialog(
                    parent=self,
                    flags=Gtk.DialogFlags.DESTROY_WITH_PARENT,
                    type=Gtk.MessageType.ERROR,
                    buttons=(
                        Gtk.STOCK_CLOSE,
                        Gtk.ButtonsType.CLOSE,
                    )
                )

                errorDialog.run()
                errorDialog.destroy()

        dialog.destroy()

    @Gtk.Template.Callback('gl_realize')
    def gl_realize(self, area):
        area.make_current()

        print("OpenGL Version: %s" % glGetString(GL_VERSION).decode())
        print("OpenGL Renderer: %s" % glGetString(GL_RENDERER).decode())

        glClearColor(0.1, 0.1, 0.1, 1)

        self.create_gl_texture()
        self.create_gl_shader()
        self.create_gl_mesh()

    def create_gl_texture(self):
        self.glTexture = glGenTextures(1)
        glBindTexture(GL_TEXTURE_2D, self.glTexture)

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
            GBx.SCREEN_WIDTH, GBx.SCREEN_HEIGHT,
            0, GL_RGB, GL_UNSIGNED_BYTE, None)

    def create_gl_shader(self):
        vertexShader = glCreateShader(GL_VERTEX_SHADER)
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER)

        glShaderSource(vertexShader, VERTEX_SHADER.encode())
        glShaderSource(fragmentShader, FRAGMENT_SHADER.encode())

        glCompileShader(vertexShader)

        if not glGetShaderiv(vertexShader, GL_COMPILE_STATUS):
            raise RuntimeError(glGetShaderInfoLog(vertexShader))

        glCompileShader(fragmentShader)

        if not glGetShaderiv(fragmentShader, GL_COMPILE_STATUS):
            raise RuntimeError(glGetShaderInfoLog(fragmentShader))

        self.glShader = glCreateProgram()

        glAttachShader(self.glShader, vertexShader)
        glAttachShader(self.glShader, fragmentShader)

        glLinkProgram(self.glShader)

        if not glGetProgramiv(self.glShader, GL_LINK_STATUS):
            raise RuntimeError(glGetProgramInfoLog(self.glShader))

        glDeleteShader(vertexShader)
        glDeleteShader(fragmentShader)

        glUseProgram(self.glShader)
    
    def create_gl_mesh(self):
        self.glVertexArray = glGenVertexArrays(1)
        glBindVertexArray(self.glVertexArray)

        self.glVertexBuffer = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.glVertexBuffer)
        glBufferData(GL_ARRAY_BUFFER, VERTICES, GL_STATIC_DRAW)

        glEnableVertexAttribArray(0)
        glVertexAttribPointer(0, 4, GL_FLOAT, False, VERTEX_SIZE, c_void_p(0))

        glEnableVertexAttribArray(1)
        glVertexAttribPointer(1, 2, GL_FLOAT, False, VERTEX_SIZE, c_void_p(POSITION_SIZE))

    @Gtk.Template.Callback('gl_unrealize')
    def gl_unrealize(self, area):
        area.make_current()

        glUseProgram(0)
        glDeleteProgram(self.glShader)
        self.glShader = 0

        glBindTexture(GL_TEXTURE_2D, 0)
        glDeleteTextures(1, self.glTexture)
        self.glTexture = 0

        glBindVertexArray(0)
        glBindBuffer(GL_ARRAY_BUFFER, 0)
        glDeleteVertexArrays(1, self.glVertexArray)
        self.glVertexArray = 0

        glBindBuffer(GL_ARRAY_BUFFER, 0)
        glDeleteBuffers(1, self.glVertexBuffer)
        self.glVertexBuffer = 0

    @Gtk.Template.Callback('gl_resize')
    def gl_resize(self, area, width, height):
        zoom = min(
            float(width) / float(GBx.SCREEN_WIDTH),
            float(height) / float(GBx.SCREEN_HEIGHT)
        )

        sizeX = float(GBx.SCREEN_WIDTH) / float(width) * zoom
        sizeY = float(GBx.SCREEN_HEIGHT) / float(height) * zoom

        offsetX = (1.0 - sizeX) * 0.5
        offsetY = (1.0 - sizeY) * 0.5

        glUniform2f(glGetUniformLocation(self.glShader, "uSize"), sizeX, sizeY)
        glUniform2f(glGetUniformLocation(self.glShader, "uOffset"), offsetX, offsetY)

    @Gtk.Template.Callback('gl_render')
    def gl_render(self, area, ctx):
        area.make_current()

        glViewport(0, 0, area.get_allocated_width(), area.get_allocated_height())

        glClear(GL_COLOR_BUFFER_BIT)

        pixels = self.GBx.PPU_GetPixelData()

        glBindTexture(GL_TEXTURE_2D, self.glTexture)
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
            GBx.SCREEN_WIDTH, GBx.SCREEN_HEIGHT,
            GL_RGB, GL_UNSIGNED_BYTE, pixels)

        glUseProgram(self.glShader)
        glBindVertexArray(self.glVertexArray)
        glDrawArrays(GL_TRIANGLES, 0, int(VERTICES.nbytes / VERTEX_SIZE))

    @Gtk.Template.Callback('toggle_fullscreen')
    def toggle_fullscreen(self, obj):
        if self.isFullscreen:
            self.unfullscreen()
            self.isFullscreen = False
        else:
            self.fullscreen()
            self.isFullscreen = True

    @Gtk.Template.Callback('zoom_2')
    def zoom_2(self, obj):
        self.set_scale(2)

    @Gtk.Template.Callback('zoom_3')
    def zoom_3(self, obj):
        self.set_scale(3)

    @Gtk.Template.Callback('zoom_4')
    def zoom_4(self, obj):
        self.set_scale(4)

    @Gtk.Template.Callback('zoom_5')
    def zoom_5(self, obj):
        self.set_scale(5)

    @Gtk.Template.Callback('zoom_6')
    def zoom_6(self, obj):
        self.set_scale(6)

    def run(self):
        self.show()

        self.isOpen = True
        while self.isOpen:
            while GLib.MainContext.default().iteration(False):
                pass

            if self.isRunning:
                self.GBx.Frame()

            self.gtkGLArea.queue_render()
        
        self.serialWindow.hide()
        self.logWindow.hide()
        self.hide()