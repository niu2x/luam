
function table.tostring(tbl, indent, limit, depth, jstack , fromLoop)

    if printxContext and not printxContext.devmode then
        return "table.tostring is not enable on prod !"
    end

    limit = limit or 1000
    depth = depth or 7
    jstack = jstack or {name = "top"}
    local i = 0

    local resultStr = nil

    local output = {}
    if type(tbl) == "table" then
        -- very important to avoid disgracing ourselves with circular referencs...
        for i, t in pairs(jstack) do
            if tbl == t then return "<" .. i .. ">,\n" end
        end
        jstack[jstack.name] = tbl

        table.insert(output, "{\n")

        local name = jstack.name
        for key, value in pairs(tbl) do
            local innerIndent = (indent or "") .. "  "
            table.insert(output, innerIndent .. tostring(key) .. " = ")
            jstack.name = name .. "." .. tostring(key)
            if type(value) == "table" and value.list then
                table.insert(output, "[CocosObject]\n") 
            else
                table.insert(
                    output, value == tbl and "<parent>," or
                        table.tostring(value, innerIndent, limit, depth, jstack , true)
                )
            end

            i = i + 1
            if i > limit then
                table.insert(output, (innerIndent or "") .. "...\n")
                break
            end
        end

        table.insert(output, indent and (indent or "") .. "},\n" or "}")
    else
        if type(tbl) == "string" then tbl = string.format("%q", tbl) end -- quote strings
        table.insert(output, tostring(tbl) .. ",\n")
    end

    

    if fromLoop then
        resultStr = table.concat(output)
    else
        resultStr = "\n[+++LuaTable+++]\n"

        -- local _output = table.concat(output) 
        -- if _output then
            resultStr = resultStr .. table.concat(output)
        -- end
        
        resultStr = resultStr .. "\n[---LuaTable---]"
    end
    
    
    return resultStr
end