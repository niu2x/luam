local _class={}

function class(super)

	local class_type = { ctor = false, super = super}    -- 'ctor' field must be here
	local vtbl = {}
	_class[class_type] = vtbl
  
    -- class_type is one proxy indeed. (return class type, but operate vtbl)
	setmetatable(class_type, {
		__newindex= function(t,k,v)

			-- Check If Already Defined !
			if(isLocalDevelopMode) then
				if rawget(vtbl, k) then
					assert(false, "Class 's Memeber \"" .. tostring(k) .. "\" Already Defined Before, This May Cause Error !")
				end
			end
		    vtbl[k] = v 
		end,			
		__index = function(t,k) 
			return vtbl[k] 
		end,
	})

    -- when first invoke the method belong to parent,retrun value of parent
    -- and set the value to child
	if super then
		setmetatable(vtbl, { 
			__index= function(t, k)

				local result = false

				-- First Check Interface
				for interface,v in pairs(class_type.interface) do
					if result == false then
						if interface[k] then
							result = interface[k]
						end
					end
				end

				if result then
					return result
				end

				-- Then Check Parent
				if k and _class[super] then
				    local ret = _class[super][k]
				    vtbl[k] = ret                      -- remove this if lua running on back-end server
				    return ret
				else return nil end
			end
		})
	end
    
    class_type.new = function(...)
        local obj = { class = class_type }
        setmetatable(obj, { 
			__index = _class[class_type]
		})
        
        -- deal constructor recursively
        local inherit_list = {}
		local class_ptr = class_type
		while class_ptr do
			if class_ptr.ctor then table.insert(inherit_list, class_ptr) end
			class_ptr = class_ptr.super
		end
		local inherit_length = #inherit_list
		if inherit_length > 0 then
		    for i = inherit_length, 1, -1 do inherit_list[i].ctor(obj, ...) end
		end
        
        obj.class = class_type              -- must be here, because some class constructor change class property.

		-------------------------
		--- Declare Variabel Check
		--------------------------
		function obj:declare(variableName, value, ...)
			assert(type(variableName) == "string", "function declare 's variableName paramter is nil !")
			assert(value ~= nil, "function declare 's value paramter is nil !")
			assert(#{...} == 0)

			assert(not self[variableName], "variable \"" .. variableName .. "\" Already Declared !")
			self[variableName] = value
		end

        return obj
    end
	
	class_type.is = function(self_ptr, compare_class, ...)
		assert(self_ptr)
		assert(compare_class)

		if not compare_class or not self_ptr then 
			return false 
		end

		local raw_class = self_ptr.class
		while raw_class do
			if raw_class == compare_class then return true end
			raw_class = raw_class.super
		end
		return false
	end

	-------------------------------------------
	--  Implement An Interface( Or Procotol Called In Cocos2d-x)
	--  -----------------------------------------------------
	class_type.interface = {}
	class_type.implement = function(interface, ...)
		assert(interface, "Interface Is Nil !!")
		assert(#{...} == 0)

		assert(_class[interface], "Interface Not Exist !!")

		-- Check If Already Added To Talbe class_type.interface
		if class_type.interface[interface] then
			assert(false, "Interface Already Implemented !")
		end

		class_type.interface[interface] = true

		return class_type
	end

	----------------------------------------------------
	--create a object with access control
	--method start with "_", will be regarded as a private function
	----------------------------------------------------
	class_type.new_ac = function()
		local delegate = {}

		local obj = class_type.new()

		local function cloneToDelegate(sourceClass)
			for k,v in pairs(sourceClass) do
				--if _G.isLocalDevelopMode then printx(0, "++++++++++++++++++++++++++++++++",k, type(v)) end
				local isPrivate = string.find(k, "_") == 1
				local isInner = k == "new" or k == "new_ac"
				if type(v) == "function" and not isPrivate and not isInner and not delegate[k] then
					--if _G.isLocalDevelopMode then printx(0, "delegated method: ", k) end
					delegate[k] = function(...)
						local params = { ... } 
						params[1] = obj
						return obj[k](unpack(params))
					end
				end
			end
		end

		cloneToDelegate(_class[class_type])

		local superClass = super
		while superClass do
			cloneToDelegate(_class[superClass])
			superClass = superClass.super
		end

		return delegate
	end
	
	return class_type
end

