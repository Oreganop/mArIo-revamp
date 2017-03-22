BoxRadius=6;
iter=400;
last_dist=3;
on_screen=0; 
fitness=0; -- 0x71A * 255 + 0x71C
was_dead=0;
skips=0;
autonomous = 1;


infile = "reactions.txt"
outfile = "observations.txt"

function getData()
	m_X = memory.readbyte(0x03AD)
	m_Y = memory.readbyte(0x03B8)
	
	
	
	local e_vector = {} -- Enemy vector
	e_vector["size"] = 0
	
	for i=0, 4 do
		local enemy_check = memory.readbyte(0xF+i)
		if enemy_check == 1 then
			local e_x = memory.readbyte(0x4B0+i*0x4)
			local e_y = memory.readbyte(0x4B1+i*0x4)-16
			e_vector[e_vector["size"]] = {["x"]=e_x, ["y"]=e_y}
			e_vector["size"]= e_vector["size"] + 1
		end
	end
	return e_vector --Gets sent to `draw()` and `writeto()`
end
	
function hex_grid_convert(val)
	return math.floor( (( val /255)*65) /4)
end

function draw(e_v)
	gui.drawBox(16,36,80,92,0xFF000000, 0x80808080)

	local cur_screen = memory.readbyte(0x71A)
	
	if on_screen > cur_screen then --So we don't have to restart the console each time mario dies... 
		on_screen = cur_screen
	end
	if on_screen < memory.readbyte(0x71A) then --So we don't have to restart the console each time mario dies... 
		on_screen = memory.readbyte(0x71A)
	end
	
	local dist_thru = memory.readbyte(0x71C)
	
	local dist_on_cur = memory.readbyte(0x5)
	
	dist_on_cur = hex_grid_convert(dist_on_cur)
	
	if dist_on_cur > 7 then -- Normalize because register fluctuates at max mario ranges
		dist_on_cur=7
	end
	
	if last_dist >5 and dist_on_cur==0 then -- Normalize because register fluctuates at max mario ranges
		dist_on_cur = last_dist
	end
	
	
	last_dist = dist_on_cur
	
	local last_tile = tile_thru
	tile_thru = math.mod(math.floor( ((dist_thru/255)*65) /4)+dist_on_cur, 16)
	if last_tile == 15 and tile_thru == 0 then
		on_screen = on_screen + 1;
	end
	
	local screen_location = hex_grid_convert(memory.readbyte(0x71C))
	
	local drew = 0;
	
	local onHUD = {}
	for i=0, 12 do
		onHUD[i]= {}
		for j=0, 15 do
			onHUD[i][j] = 0;
		end
	end
	
	for dy=0, 12 do
		for dx=0, 15 do
		
			--[[ Draw tiles...
				Tiles are housed in registers 0x500 to 0x69F. 
				This means there are 2 "screens" loaded at all times -- the current one, and the next one.
				
				Screens are 15x by 14y.
				
				variable "on_screen" tells us which screen mario is standing on. Even will be 0x500 -> 0x5CF
																				  Odd will be 0x5D0 -> 0x69F
			]]
			
			
			local x_around_m = ( screen_location + dx)  
			if x_around_m > 15 then 
				x_around_m = (x_around_m % 16) + 208
			end	
			
			
			
			local address = 0x500 + x_around_m + (dy)*16 + (208 * (cur_screen%2))
			--console.log(address)
		
			if address > 0x69F then
				address = address % 0x69F
				address = address + 0x500
			end
			tile_data = memory.readbyte(address)
			if tile_data == 0xc0 or tile_data == 0xc1 or tile_data == 0xc1  then 
				-- Unused Question block.
				onHUD[dy][dx] = 40;
				gui.drawBox(16+dx*4, 36+(dy+1)*4, 20+dx*4, 40+(dy+1)*4, 0xFFFF8C00, 0x88FF8C00 )
			end
			if tile_data == 0xc4 then 
				-- Used Question block.
				onHUD[dy][dx] = 42;
				gui.drawBox(16+dx*4, 36+(dy+1)*4, 20+dx*4, 40+(dy+1)*4, 0xFFC0C0C0, 0x88C0C0C0  )
			end
			if tile_data == 0x51 or tile_data == 0x52 then 
				onHUD[dy][dx] = 28;
				-- Bricks
				gui.drawBox(16+dx*4, 36+(dy+1)*4, 20+dx*4, 40+(dy+1)*4, 0xFF841F27, 0x88841F27  )
			end
			if tile_data == 0x54 then 
				onHUD[dy][dx] = 20;
				-- Unbreakable block
				gui.drawBox(16+dx*4, 36+(dy+1)*4, 20+dx*4, 40+(dy+1)*4, 0xFFc2b280, 0x88c2b280 )
			end
			if tile_data == 0x12 or tile_data == 0x13 then 
				-- Warp top
				onHUD[dy][dx] = 60;
				gui.drawBox(16+dx*4, 36+(dy+1)*4, 20+dx*4, 40+(dy+1)*4, 0xFF32cd32, 0x8832cd32 )
			end 
			if tile_data == 0x1c or tile_data == 0x1f then 
				-- Functional sideways warp
				onHUD[dy][dx] = 62;
				gui.drawBox(16+dx*4, 36+(dy+1)*4, 20+dx*4, 40+(dy+1)*4, 0xFF32ee32, 0x8832ee32 )
			end 
			if tile_data == 0x10 or tile_data == 0x11 then 
				-- Functional warp
				onHUD[dy][dx] = 64;
				gui.drawBox(16+dx*4, 36+(dy+1)*4, 20+dx*4, 40+(dy+1)*4, 0xFF32ee32, 0x8832ee32 )
			end 
			if tile_data == 0x14 or tile_data == 0x15 or tile_data == 0x1d or tile_data == 0x20 then 
				-- Warp bot
				onHUD[dy][dx] = 66;
				gui.drawBox(16+dx*4, 36+(dy+1)*4, 20+dx*4, 40+(dy+1)*4, 0xFF2db82d, 0x882db82d )
			end 
			if tile_data == 0x61 then 
				-- Square block
				onHUD[dy][dx] = 80;
				gui.drawBox(16+dx*4, 36+(dy+1)*4, 20+dx*4, 40+(dy+1)*4, 0xFFF4A460, 0x88F4A460 )
			end 
			if tile_data == 0x5d or tile_data == 0x58 or tile_data == 0x57 or  tile_data == 0x5a or tile_data == 0x5e  then 
				-- Special Square block
				onHUD[dy][dx] = 82;
				gui.drawBox(16+dx*4, 36+(dy+1)*4, 20+dx*4, 40+(dy+1)*4, 0xFFBC8F8F, 0x88BC8F8F )
			end 
		end
	end
	for i=0, e_v["size"]-1 do
		local posx = hex_grid_convert(e_v[i]["x"])
		local posy = hex_grid_convert(e_v[i]["y"])
		if posx < 16 and posx>=0 and posy >0 and posy <= 12 then -- posy actually needs to be less than that, but we'll never encounter that...	
			onHUD[posy-1][posx] = onHUD[posy-1][posx] + 10;
			gui.drawBox(16+posx*4, 36+posy*4,20+posx*4, 40+posy*4, 0xFF440000, 0x88FF0000)
		end
	end
	
	-- Do Mario
	local posx = math.floor( ((m_X/255)*65) /4)
	local posy = math.floor( ((m_Y/255)*65) /4)
	gui.drawText(81,80, string.format("%i %i", posx, posy))
	--gui.drawText(80,80, string.format("Fucks I give: %i", 0))


	if posx < 16 and posx>=0 and posy >0 and posy <= 12 then -- posy actually needs to be less than that, but we'll never encounter that...	
		onHUD[posy-1][posx] = onHUD[posy-1][posx] + 31;
		if memory.readbyte(0x756) >= 1 then
			onHUD[posy-2][posx] = onHUD[posy-1][posx] + 31;
			gui.drawBox(16+posx*4, 36+(posy-1)*4,20+posx*4, 40+(posy-1)*4, 0xFF007700, 0x8800FF00)
		end
	end
	gui.drawBox(16+posx*4, 36+posy*4,20+posx*4, 40+posy*4, 0xFF007700, 0x8800FF00)
	
	return onHUD
end

function writeto(HUD)
	local f = assert(io.open(outfile, "w"))--1280 -> 1695
	
	if memory.readbyte(0x7f8) > 0 and memory.readbyte(0x7f8) < 4 then 
		memory.write_s8(0x7f8, 0)
		memory.write_s8(0x7f9, 5)
		skips=0;
	end
	
	
	if memory.readbyte(0x7b1) == 1 or (memory.readbyte(0x7F8)==0 and memory.readbyte(0x7f9)==0 and memory.readbyte(0x7fa)==0) then
		if was_dead == 0 then 
			f:write(string.format("%s", "DEAD\n"))
			--console.log("kekekek")
			f:write(string.format("%i", memory.readbyte(0x71A) * 255 + memory.readbyte(0x71C) + memory.readbyte(0x760)*5000))
			f:close();
			was_dead=1;
			memory.write_s8(0x71A,0)
		end
		return
	end
	was_dead=0;
	f:write(string.format("%i\n", iter))
	
	for i=0, #HUD do
		for j=0,#HUD[i] do
			local put_string = string.format("%02i ", HUD[i][j]);
			f:write(put_string)
		end
		--For testing output.
		--[[
		put_string = string.format("\n");
		f:write(put_string)
		]]
	end
	put_string = string.format("\n");
	f:write(put_string)
	put_string = string.format("%i", iter)
	f:write(put_string);

	f:close();
end

function readfrom()
	-- TODO change so that gets inputs from input.txt
	
	local cur_contoller = joypad.get(1)
		
	local f = assert(io.open(infile, "r"))
	local got 

    repeat
        got = f:read("*all")
    until got ~= nil and got ~= '' 
	
    for key, _ in pairs(cur_contoller) do
        cur_contoller[key] = "False"
    end

    local ypos = 0;
    for input in got:gmatch("%a+") do
        cur_contoller[input] = "True"
        gui.drawText(80, 36+ypos*14, input)
        ypos=ypos+1;
    end
	
	
	--console.log(cur_contoller)
	
	f:close()
	return cur_contoller
end 
		
while true do
	if iter == 800 then
		iter = 400
	end
	iter = iter + 1
	local e_vector = getData();
	local HUD = draw(e_vector);
	writeto(HUD)
	
		memory.write_s8(0x75A, 48)
	local controller = readfrom();
	if autonomous==1 then
		joypad.set(controller, 1)
	end
	emu.frameadvance();
	
	--Example of how we can get input into a controller.
--[[
	local cur_contoller = joypad.get(1) 
	--console.log(cur_contoller)
	if cur_contoller ~= nil then
		cur_contoller["Right"] = "True"	
	end
	--console.log(cur_contoller)
	joypad.set(cur_contoller, 1)
	]]
end
