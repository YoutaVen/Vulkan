# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

# <pep8-80 compliant>

bl_info = {
    "name": "VulKan ToolS (VKTS) format",
    "author": "Norbert Nopper",
    "blender": (2, 73, 0),
    "location": "File > Export",
    "description": "Export as VKTS scene data",
    "warning": "",
    "wiki_url": ""
                "",
    "support": 'COMMUNITY',
    "category": "Export"}

# Copyright (C) 2015: Norbert Nopper

# To support reload properly, try to access a package var,
# if it's there, reload everything
if "bpy" in locals():
    import imp
    if "export_vkts" in locals():
        imp.reload(export_vkts)


import os
import bpy
from bpy.props import (CollectionProperty,
                       StringProperty,
                       BoolProperty,
                       EnumProperty,
                       FloatProperty,
                       )
from bpy_extras.io_utils import (ExportHelper,
                                 axis_conversion,
                                 )


class ExportVKTS(bpy.types.Operator, ExportHelper):
    """Exports scene into the VulKan ToolS (VKTS) format"""
    bl_idname = "export_scene.vkts"
    bl_label = "Export VKTS"

    filename_ext = ".vkts"
    filter_glob = StringProperty(default="*.vkts", options={'HIDDEN'})

    axis_forward = EnumProperty(
            name="Forward",
            items=(('X', "X Forward", ""),
                   ('Y', "Y Forward", ""),
                   ('Z', "Z Forward", ""),
                   ('-X', "-X Forward", ""),
                   ('-Y', "-Y Forward", ""),
                   ('-Z', "-Z Forward", ""),
                   ),
            default='-Z',
            )
    axis_up = EnumProperty(
            name="Up",
            items=(('X', "X Up", ""),
                   ('Y', "Y Up", ""),
                   ('Z', "Z Up", ""),
                   ('-X', "-X Up", ""),
                   ('-Y', "-Y Up", ""),
                   ('-Z', "-Z Up", ""),
                   ),
            default='Y',
            )

    @classmethod
    def poll(cls, context):
        return True

    def execute(self, context):
        from . import export_vkts

        keywords = self.as_keywords(ignore=("axis_forward",
                                            "axis_up",
                                            "global_scale",
                                            "check_existing",
                                            "filter_glob",
                                            ))

        global_matrix = axis_conversion(to_forward=self.axis_forward,
                                        to_up=self.axis_up,
                                        ).to_4x4()
        keywords["global_matrix"] = global_matrix
        
        filepath = self.filepath
        filepath = bpy.path.ensure_ext(filepath, self.filename_ext)

        return export_vkts.save(self, context, **keywords)

    def draw(self, context):
        layout = self.layout

        layout.prop(self, "axis_forward")
        layout.prop(self, "axis_up")


def menu_func_export(self, context):
    self.layout.operator(ExportVKTS.bl_idname, text="VKTS (.vkts)")


def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
    register()
