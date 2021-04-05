
import os

import gi
gi.require_version("Gtk", "3.0")
from gi.repository import Gio

filename = os.path.join(os.path.dirname(__file__), "gbx.gresource")
resource = Gio.Resource.load(filename)

# https://bugzilla.gnome.org/show_bug.cgi?id=684319
resource._register()