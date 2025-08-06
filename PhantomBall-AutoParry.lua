local Rayfield = loadstring(game:HttpGet("https://sirius.menu/rayfield"))()

local Players = game:GetService("Players")
local RunService = game:GetService("RunService")
local UserInputService = game:GetService("UserInputService")
local StarterGui = game:GetService("StarterGui")
local TweenService = game:GetService("TweenService")
local ReplicatedStorage = game:GetService("ReplicatedStorage")

local plr = Players.LocalPlayer
local chr = plr.Character or plr.CharacterAdded:Wait()
local hrp = chr:WaitForChild("HumanoidRootPart")
local ball = workspace:WaitForChild("GameBall")

-- Remote parry
local remote = ReplicatedStorage:WaitForChild("TS"):WaitForChild("GeneratedNetworkRemotes"):WaitForChild("RE_4.6848415795802784e+76")
local args = { 2.933813859058389e+76 }

local function parry()
	remote:FireServer(unpack(args))
end

local BASE_DIST = 36
local curMax = BASE_DIST
local lastTick = tick()
local ballPressed = false

local SAFE_DIST = 50
local TWEEN_TIME = 0.3
local TWEEN_DEBOUNCE = 0.5
local lastTween = 0
local isTweening = false

local AutoParryEnabled = false
local SafeDistanceEnabled = false

local function hasHighlight(model)
	return model and model:FindFirstChildWhichIsA("Highlight") ~= nil
end

local function tweenToPosition(targetPos)
	if not hrp or not hrp.Parent then return end
	if tick() - lastTween < TWEEN_DEBOUNCE then return end
	lastTween = tick()
	isTweening = true
	local tweenInfo = TweenInfo.new(TWEEN_TIME, Enum.EasingStyle.Quad, Enum.EasingDirection.Out)
	local goal = {CFrame = CFrame.new(targetPos)}
	local tw = TweenService:Create(hrp, tweenInfo, goal)
	tw:Play()
	tw.Completed:Wait()
	isTweening = false
end

local function ensureSafeDistanceAllOnce()
	if not hrp or not hrp.Parent then return end
	for _, p in pairs(Players:GetPlayers()) do
		if p ~= plr and p.Character and p.Character:FindFirstChild("HumanoidRootPart") then
			local otherHRP = p.Character.HumanoidRootPart
			local dir = hrp.Position - otherHRP.Position
			local dist = dir.Magnitude
			if dist < SAFE_DIST then
				if dist <= 0.001 then
					dir = Vector3.new(math.random()-0.5, 0, math.random()-0.5)
				end
				local unit = dir.Unit
				local targetPos = otherHRP.Position + unit * SAFE_DIST
				targetPos = Vector3.new(targetPos.X, hrp.Position.Y, targetPos.Z)
				tweenToPosition(targetPos)
			end
		end
	end
end

local function resetDistance()
	curMax = BASE_DIST
	ballPressed = false
end

local function hookDied(char)
	local hum = char:FindFirstChildOfClass("Humanoid")
	if hum then
		hum.HealthChanged:Connect(function(h)
			if h <= 0 then
				resetDistance()
				if SafeDistanceEnabled then
					ensureSafeDistanceAllOnce()
				end
			end
		end)
	end
end

for _, p in pairs(Players:GetPlayers()) do
	if p.Character then hookDied(p.Character) end
	p.CharacterAdded:Connect(function(c) hookDied(c) end)
end
Players.PlayerAdded:Connect(function(p)
	p.CharacterAdded:Connect(function(c) hookDied(c) end)
end)

plr.CharacterAdded:Connect(function(c)
	chr = c
	hrp = chr:WaitForChild("HumanoidRootPart")
	ballPressed = false
end)

RunService.Heartbeat:Connect(function()
	if not chr or not hrp then return end

	if AutoParryEnabled then
		local dist = (ball.Position - hrp.Position).Magnitude
		local inRange = dist <= curMax
		local highlight = hasHighlight(chr)

		if highlight then
			-- Nếu trong range và chưa parry cho lần highlight này -> parry 1 phát
			if inRange and not ballPressed then
				parry()
				ballPressed = true
			end
			-- Nếu ngoài range thì chờ, không parry
			if not inRange then
				ballPressed = false
			end
		else
			ballPressed = false
		end

		-- Giữ nguyên tăng distance
		if tick() - lastTick >= 0.1 then
			curMax = curMax + 0.4
			lastTick = tick()
		end
	end

	if SafeDistanceEnabled and not isTweening and tick() - lastTween >= TWEEN_DEBOUNCE then
		for _, p in pairs(Players:GetPlayers()) do
			if p ~= plr and p.Character and p.Character:FindFirstChild("HumanoidRootPart") then
				if (p.Character.HumanoidRootPart.Position - hrp.Position).Magnitude < SAFE_DIST then
					ensureSafeDistanceAllOnce()
					break
				end
			end
		end
	end
end)

UserInputService.InputBegan:Connect(function(input, gpe)
	if gpe then return end
	if input.KeyCode == Enum.KeyCode.RightControl then
		resetDistance()
	end
end)

local Window = Rayfield:CreateWindow({
	Name = "AutoParry + SafeDistance",
	LoadingTitle = "Loaded",
	LoadingSubtitle = "Phantomball auto parry",
	ConfigurationSaving = {
		Enabled = true,
		FolderName = "AutoParryConfigs",
		FileName = "config"
	}
})

local MainTab = Window:CreateTab("Main", 4483362458)

MainTab:CreateToggle({
	Name = "AutoParry",
	CurrentValue = false,
	Flag = "AutoParryToggle",
	Callback = function(val)
		AutoParryEnabled = val
		if not val then
			resetDistance()
		end
	end
})

MainTab:CreateToggle({
	Name = "Safe Distance (50 stud)",
	CurrentValue = false,
	Flag = "SafeDistanceToggle",
	Callback = function(val)
		SafeDistanceEnabled = val
		if val then
			ensureSafeDistanceAllOnce()
		end
	end
})

MainTab:CreateButton({
	Name = "Reset Distance(must use when start game)",
	Callback = function()
		resetDistance()
	end
})
