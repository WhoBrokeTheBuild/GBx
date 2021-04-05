
import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk, Gio

from .libGBx import GBx

@Gtk.Template(resource_path="/GBx/SerialWindow.ui")
class SerialWindow(Gtk.Window):
    __gtype_name__ = 'GBxSerialWindow'

    gtkTextBufferText = Gtk.Template.Child()
    gtkTextBufferHex = Gtk.Template.Child()

    gtkAutoScroll = Gtk.Template.Child()

    gtkTextViewText = Gtk.Template.Child()
    gtkTextViewHex = Gtk.Template.Child()

    def __init__(self, app):
        super(SerialWindow, self).__init__()

        self.app = app
        self.app.GBx.Serial_SetCallbacks(self.read_byte, None, self)

    @Gtk.Template.Callback('on_destroy')
    def on_destroy(self, obj):
        self.app.GBx.Serial_SetCallbacks(None, None, None)

    @Gtk.Template.Callback('clear')
    def clear(self, obj):
        self.gtkTextViewText.set_text('')
        self.gtkTextViewHex.set_text('')

    @GBx.SERIAL_READ_FUNC
    def read_byte(self, byte):
        end = self.gtkTextBufferText.get_end_iter()
        self.gtkTextBufferText.insert(end, '%c' % (chr(byte)))
        
        end = self.gtkTextBufferHex.get_end_iter()
        self.gtkTextBufferHex.insert(end, '%02X ' % (byte))
        
        if self.gtkAutoScroll.get_active():
            mark = self.gtkTextBufferText.get_insert()
            self.gtkTextViewText.scroll_to_mark(mark, 0, False, 0, False)

            mark = self.gtkTextBufferHex.get_insert()
            self.gtkTextViewHex.scroll_to_mark(mark, 0, False, 0, False)