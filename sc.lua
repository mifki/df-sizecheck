local checkedt = {}
local checkeda = {}

local queue = { df.global.world }

local function check_r(obj)
	for k,v in pairs(obj) do
		if df.isvalid(v) == 'ref' then
			if v and v._kind == 'container' then
				table.insert(queue, v)
			
			elseif v and v._kind == 'struct' then
				local s, a = v:sizeof()
				local t = v._type

				if obj:_field(k)._kind == 'primitive' and df.reinterpret_cast('uint32_t',a-8).value == 0x11223344 then
					if not checkedt[t] then
						checkedt[t] = true
						local s2 = df.reinterpret_cast('uint64_t',a-16).value
						if s2 == s then
							--print ('  OK')
						else
							print (t)
							print ('  NOT OK '.. s .. '  ' .. s2)
						end
					end

					if df.reinterpret_cast('uint32_t',a-4).value ~= 0x44332211 then
						df.reinterpret_cast('uint32_t',a-4).value = 0x44332211
						table.insert(queue, v)
					end

				elseif not checkeda[a] then
					checkeda[a] = true
					table.insert(queue, v)
				end
			end
		end
	end
end

while #queue > 0 do
	local v = queue[#queue]
	--print (v)
	table.remove(queue, #queue)
	check_r(v)
end