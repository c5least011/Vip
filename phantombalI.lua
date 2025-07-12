local Players = game:GetService("Players")    
local TweenService = game:GetService("TweenService")    
local VIM = game:GetService("VirtualInputManager")    
local lp = Players.LocalPlayer    
    
local Rayfield = loadstring(game:HttpGet("https://sirius.menu/rayfield"))()    
    
local Window = Rayfield:CreateWindow({    
	Name = "Phantom Ball",    
	LoadingTitle = "Skibidi",    
	LoadingSubtitle = "Skibidi",    
	ConfigurationSaving = {Enabled = false},    
	Discord = {Enabled = false},    
	KeySystem = false    
})    
    
local MainTab = Window:CreateTab("Main", 4483362458)    
local KeyTab = Window:CreateTab("Keybinds", 4483362458)    
    
local flying, spamE, tpBall = false, false, false    
local lastParry = 0    
local originalColor = nil    
local changed = false    
local angle = 0    
local pressKey = Enum.KeyCode.F    
    
MainTab:CreateToggle({    
	Name = "Follow Ball",    
	CurrentValue = false,    
	Callback = function(v) flying = v end    
})    
    
MainTab:CreateToggle({    
	Name = "Spam Block",    
	CurrentValue = false,    
	Callback = function(v) spamE = v end    
})    
    
MainTab:CreateToggle({    
	Name = "TP Ball",    
	CurrentValue = false,    
	Callback = function(v) tpBall = v end    
})    
    
MainTab:CreateButton({    
	Name = "Tween to Play Area",    
	Callback = function()    
		local char = lp.Character    
		if not char or not char:FindFirstChild("HumanoidRootPart") then return end    
		local pos = Vector3.new(-283.0353698730469, 202.38238525390625, -33.97831726074219)    
		local hrp = char.HumanoidRootPart    
		local dist = (hrp.Position - pos).Magnitude    
		TweenService:Create(hrp, TweenInfo.new(dist / 100, Enum.EasingStyle.Linear), { Position = pos }):Play()    
	end    
})    
    
MainTab:CreateButton({    
	Name = "Force Reset",    
	Callback = function()    
		local char = lp.Character    
		if char then    
			local hum = char:FindFirstChildOfClass("Humanoid")    
			if hum then hum.Health = 0 end    
		end    
	end    
})    
    
-- GUI Click Key    
local ScreenGui = Instance.new("ScreenGui", game:GetService("CoreGui"))    
ScreenGui.Name = "ForceE_UI"    
ScreenGui.Enabled = false    
    
local button = Instance.new("TextButton", ScreenGui)    
button.Size = UDim2.new(0, 100, 0, 40)    
button.Position = UDim2.new(0.5, -50, 0.9, 0)    
button.BackgroundColor3 = Color3.fromRGB(255, 80, 80)    
button.TextColor3 = Color3.new(1, 1, 1)    
button.Font = Enum.Font.GothamBold    
button.TextSize = 14    
button.Text = "Block Ui"    
    
button.MouseButton1Click:Connect(function()    
	lastParry = tick()    
	changed = false    
	VIM:SendKeyEvent(true, pressKey, false, game)    
	task.wait(0.05)    
	VIM:SendKeyEvent(false, pressKey, false, game)    
	game.StarterGui:SetCore("SendNotification", {    
		Title = "Script",    
		Text = tostring(pressKey).." clicked",    
		Duration = 1    
	})    
end)    
    
MainTab:CreateToggle({    
	Name = "Show Block Ui",    
	CurrentValue = false,    
	Callback = function(v) ScreenGui.Enabled = v end    
})    
    
-- Spam Key    
task.spawn(function()    
	while task.wait(0.001) do    
		if spamE then    
			VIM:SendKeyEvent(true, pressKey, false, game)    
			task.wait(0.05)    
			VIM:SendKeyEvent(false, pressKey, false, game)    
		end    
	end    
end)    
    
-- Follow Ball Fly    
task.spawn(function()  
	while task.wait() do  
		if not flying then continue end  
		if tick() - lastParry < 0.03 then continue end  
		local char = lp.Character  
		if not char or not char:FindFirstChild("HumanoidRootPart") then continue end  
		local hrp = char.HumanoidRootPart  
		local ball = workspace:FindFirstChild("GameBall")  
		if not ball then continue end  
		local color = ball.Color  
		if not originalColor then originalColor = color end  
		if color ~= originalColor and not changed then  
			changed = true  
			lastParry = tick()  
			game.StarterGui:SetCore("SendNotification", {
				Title = "Ball Detect",
				Text = "Fly + Click",
				Duration = 1
			})  
			VIM:SendKeyEvent(true, pressKey, false, game)  
			task.wait(0.05)  
			VIM:SendKeyEvent(false, pressKey, false, game)  
		end  
		if color == originalColor and changed then  
			changed = false  
		end  
		local radius = changed and 20 or 50  
		angle += math.rad(10)  
		local x = math.cos(angle) * radius  
		local z = math.sin(angle) * radius  
		local offset = Vector3.new(x, 0, z)  
		local predictedPos = ball.Position + ball.AssemblyLinearVelocity * 0.2  
		local targetPos = predictedPos + offset  
		TweenService:Create(hrp, TweenInfo.new(0.01, Enum.EasingStyle.Linear), {
			Position = targetPos
		}):Play()  
	end  
end)  
  
-- TP Ball Detect  
task.spawn(function()  
	while task.wait() do  
		if not tpBall then continue end  
		if tick() - lastParry < 0.03 then continue end  
		local char = lp.Character  
		if not char or not char:FindFirstChild("HumanoidRootPart") then continue end  
		local hrp = char.HumanoidRootPart  
		local ball = workspace:FindFirstChild("GameBall")  
		if not ball then continue end  
		local color = ball.Color  
		if not originalColor then originalColor = color end  
		if color ~= originalColor then  
			lastParry = tick()  
			originalColor = color  
			game.StarterGui:SetCore("SendNotification", {
				Title = "TP Ball",
				Text = "Ball changed! TP + Click!",
				Duration = 1
			})  
			VIM:SendKeyEvent(true, pressKey, false, game)  
			VIM:SendKeyEvent(false, pressKey, false, game)  
			local dir = (hrp.Position - ball.Position).Unit  
			local pos = ball.Position + dir * 4  
			hrp.CFrame = CFrame.new(pos)  
		end  
	end  
end)  
  
-- Keybinds  
KeyTab:CreateButton({    
	Name = "Click F",    
	Callback = function()    
		pressKey = Enum.KeyCode.F    
	end    
})    
    
KeyTab:CreateButton({    
	Name = "Click E",    
	Callback = function()    
		pressKey = Enum.KeyCode.E    
	end    
})
