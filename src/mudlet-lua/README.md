This is the source of truth for Mudlet lua code ([mudlet-lua](https://github.com/Mudlet/Mudlet/tree/development/src/mudlet-lua) folder).

Project structure:
------------------

    mudlet-lua/
        lua/                  all Lua code
            luadoc/           LuaDoc extensions (this code is not loaded by Mudlet)
            LuaGlobal.lua     loader for all Lua code used by Mudlet

        tests/                unit tests for mudlet-lua 
        mudlet-lua-doc/       generated documentation

        CONTRIBS
        genDoc.sh             script for generation documentation
        luadoc-guide.txt      guide with basic LuaDoc conventions
        README
        TODO

Current unit tests status: [![Build Status](https://travis-ci.org/vadi2/mudlet-lua.png?branch=master)](https://travis-ci.org/vadi2/mudlet-lua)

MXP events
----------

`mxp.send` events now include a `caption` field with the text between the SEND tags.

The `caption` is available via `mxp.send.caption` and contains the plain text
displayed when hovering over the element. Commands triggered by the SEND
element continue to be listed in `mxp.send.actions`, while the optional `href`
attribute is stored in `mxp.send.href` if present. The nested
`actions.caption` field that previously held this text no longer exists.
