local ecs = require "ecs"

local function rand_flt(from, to)
	local maxNumber = 32767
    return from + (math.random(maxNumber) / maxNumber) * (to - from)
end

local function move(it)
    for pos, vel, ent in ecs.each(it) do
        pos.x = pos.x + vel.x * it.delta_time
        pos.y = pos.y + vel.y * it.delta_time
		pos.z = pos.z + vel.z * it.delta_time
    end
end

local function gravity(it)
    for pos, vel, grav, plane, ent in ecs.each(it) do
        local planeEpsilon = 0.1
		
		if plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon then
			do return end
		end
		
		vel.x = vel.x + grav.x * it.delta_time
        vel.y = vel.y + grav.y * it.delta_time
		vel.z = vel.z + grav.z * it.delta_time
    end
end

local function FrictionSystem(it)
    for vel, friction, ent in ecs.each(it) do
        vel.x = vel.x - vel.x * friction.value * it.delta_time
		vel.y = vel.y - vel.y * friction.value * it.delta_time
		vel.z = vel.z - vel.z * friction.value * it.delta_time
    end
end

local function ShiverSystem(it)
    for pos, shiver, ent in ecs.each(it) do
        pos.x = pos.x + rand_flt(-shiver.value, shiver.value)
		pos.y = pos.y + rand_flt(-shiver.value, shiver.value)
		pos.z = pos.z + rand_flt(-shiver.value, shiver.value)
    end
end

local function BounceSystem(it)
    for pos, vel, plane, bounciness, ent in ecs.each(it) do
        local dotPos = plane.x * pos.x + plane.y * pos.y + plane.z * pos.z
		local dotVel = plane.x * vel.x + plane.y * vel.y + plane.z * vel.z
		
		if dotPos < plane.w then
			pos.x = pos.x - (dotPos - plane.w) * plane.x
			pos.y = pos.y - (dotPos - plane.w) * plane.y
			pos.z = pos.z - (dotPos - plane.w) * plane.z

			vel.x = vel.x - (1.0 + bounciness.value) * plane.x * dotVel
			vel.y = vel.y - (1.0 + bounciness.value) * plane.y * dotVel
			vel.z = vel.z - (1.0 + bounciness.value) * plane.z * dotVel
		end
    end
end


local function DestructionOnTimer(it)
	for pos, timer in ecs.each(it) do
		timer.timeLeft = timer.timeLeft - it.delta_time
		if timer.timeLeft <= 0.0 then
			pos.x = 10000.0
			pos.y = 10000.0
			pos.z = 10000.0
		end
	end
end


local function Check1DLineIntersection(a1, b1, a2, b2)
	return (a1 <= b2) and (b1 <= a2)
end


local function CheckCuboidIntersection(pos1, collider1, pos2, collider2)
	checkX = Check1DLineIntersection(pos1.x - collider1.extentX, pos1.x + collider1.extentX, pos2.x - collider2.extentX, pos2.x + collider2.extentX)
	checkY = Check1DLineIntersection(pos1.y - collider1.extentY, pos1.y + collider1.extentY, pos2.y - collider2.extentY, pos2.y + collider2.extentY)
	checkZ = Check1DLineIntersection(pos1.z - collider1.extentZ, pos1.z + collider1.extentZ, pos2.z - collider2.extentZ, pos2.z + collider2.extentZ)
	return checkX and checkY and checkZ
end


local function KillOnCollision(it)
	for pos1, collider1, killOnCollision in ecs.each(it) do
		local q = ecs.query("Position, Collider, DieOnCollision")
		local it = ecs.query_iter(q)
		for pos2, collider2, dieOnCollision in ecs.each(it) do
			if CheckCuboidIntersection(pos1, collider1, pos2, collider2) then
				pos2.x = 10000.0
				pos2.y = 10000.0
				pos2.z = 10000.0
			end
		end
	end
end


ecs.system(move, "Move", ecs.OnUpdate, "Position, Velocity")
ecs.system(gravity, "grav", ecs.OnUpdate, "Position, Velocity, Gravity, BouncePlane")
ecs.system(FrictionSystem, "FrictionSystem", ecs.OnUpdate, "Velocity, FrictionAmount")
ecs.system(ShiverSystem, "ShiverSystem", ecs.OnUpdate, "Position, ShiverAmount")
ecs.system(BounceSystem, "BounceSystem", ecs.OnUpdate, "Position, Velocity, BouncePlane, Bounciness")
ecs.system(DestructionOnTimer, "DestructionOnTimerSystem", ecs.OnUpdate, "Position, DestructionTimer")
ecs.system(KillerOnCollision, "DeathOnCollisionSystem", ecs.OnUpdate, "Position, Collider, KillOnCollision")
