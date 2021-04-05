
import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk

from .libGBx import clog

clog_color_map = [
    'default',
    'black',
    'red',
    'green',
    'orange', # Technically yellow
    'blue',
    'magenta',
    'cyan',
    'white',
]

@Gtk.Template(resource_path="/GBx/LogWindow.ui")
class LogWindow(Gtk.Window):
    __gtype_name__ = 'GBxLogWindow'

    gtkTextBuffer = Gtk.Template.Child()
    gtkTextView = Gtk.Template.Child()
    gtkAutoScroll = Gtk.Template.Child()

    def __init__(self):
        super(LogWindow, self).__init__()

        clog.init()
        clog.add_callback(self.add_entry, self)

    @Gtk.Template.Callback('on_destroy')
    def on_destroy(self, obj):
        clog.remove_callback(self.add_entry, self)
        clog.term()

    @Gtk.Template.Callback('clear')
    def clear(self, obj):
        self.gtkTextBuffer.set_text('')
            
    @clog.CALLBACK_FUNC
    def add_entry(self, color, message):
        end = self.gtkTextBuffer.get_end_iter()

        if color > 0:
            colorName = clog_color_map[color]
            buffer = '<span color="%s">%s</span>' % (colorName, message.decode())
            self.gtkTextBuffer.insert_markup(end, buffer, -1)
        else:
            self.gtkTextBuffer.insert(end, message.decode())

        if self.gtkAutoScroll.get_active():
            mark = self.gtkTextBuffer.get_insert()
            self.gtkTextView.scroll_to_mark(mark, 0, False, 0, False)
