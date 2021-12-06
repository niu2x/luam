require 'class'

local function valid(node)
	return type(node) == 'table'
end

ast_visitor_t = class()

function ast_visitor_t:visit(ast)
	if not valid(ast) then return end

	local type = ast.type
	print('type', ast.type)

	self[type .. '_begin'](self, ast)
	self['visit_' .. type](self, ast)
	self[type .. '_end'](self, ast)
end

function ast_visitor_t:visit_block(node)
	self:visit(node.chunk)
end

function ast_visitor_t:visit_chunk(node)
	self:visit(node.statlist)
	self:visit(node.laststatpart)
end

function ast_visitor_t:visit_statlist(node)
	self:visit(node.statlist)
	self:visit(node.statpart)
end

function ast_visitor_t:visit_laststatpart(node)
	self:visit(node.laststat)
end

function ast_visitor_t:visit_laststat(node)
	if node.subType == 'return' then
		self:visit(node.explist)
	end
end

function ast_visitor_t:visit_statpart(node)
	self:visit(node.stat)
end

function ast_visitor_t:visit_stat(node)
	-- self:visit(node.stat)
	if node.subType == 'varlist_assgin_explist' then
		self:visit(node.varlist)
		self:visit(node.explist)
	end

	if node.subType == 'functioncall' then
		self:visit(node.functioncall)
	end

	
end

function ast_visitor_t:block_begin(node)
end
function ast_visitor_t:block_end(node)
end

function ast_visitor_t:chunk_begin(node)
end
function ast_visitor_t:chunk_end(node)
end

function ast_visitor_t:statlist_begin(node)
end
function ast_visitor_t:statlist_end(node)
end

function ast_visitor_t:statpart_begin(node)
end
function ast_visitor_t:statpart_end(node)
end

function ast_visitor_t:stat_begin(node)
end
function ast_visitor_t:stat_end(node)
end
