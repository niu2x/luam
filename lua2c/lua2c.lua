require('utils')
local json = require('./json')
require 'visitor'

local ast = io.read('*all')
ast = json.decode(ast)


-- print(table.tostring(ast))

local my_visitor = ast_visitor_t.new()
my_visitor:visit(ast)