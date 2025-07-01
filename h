local lp = game.Players.LocalPlayer
local gui = Instance.new("ScreenGui", lp:WaitForChild("PlayerGui"))
gui.Name = "C5_GUI"
gui.ResetOnSpawn = false

local main = Instance.new("Frame", gui)
main.Size = UDim2.new(0, 450, 0, 320)
main.Position = UDim2.new(0.5, -225, 0.5, -160)
main.BackgroundColor3 = Color3.fromRGB(30, 30, 30)
main.Active = true
main.Draggable = true
Instance.new("UICorner", main).CornerRadius = UDim.new(0, 10)

local tabButtons = Instance.new("Frame", main)
tabButtons.Size = UDim2.new(1, 0, 0, 30)
tabButtons.BackgroundColor3 = Color3.fromRGB(40, 40, 40)

local tabLayout = Instance.new("UIListLayout", tabButtons)
tabLayout.FillDirection = Enum.FillDirection.Horizontal
tabLayout.SortOrder = Enum.SortOrder.LayoutOrder
tabLayout.HorizontalAlignment = Enum.HorizontalAlignment.Left
tabLayout.Padding = UDim.new(0, 2)

local tabContainer = Instance.new("Frame", main)
tabContainer.Position = UDim2.new(0, 0, 0, 30)
tabContainer.Size = UDim2.new(1, 0, 1, -30)
tabContainer.BackgroundColor3 = Color3.fromRGB(25, 25, 25)

local tabs = {}
local function createTab(name)
	local btn = Instance.new("TextButton", tabButtons)
	btn.Text = name
	btn.Size = UDim2.new(0, 0, 1, 0)
	btn.AutomaticSize = Enum.AutomaticSize.X
	btn.BackgroundColor3 = Color3.fromRGB(50, 50, 50)
	btn.TextColor3 = Color3.new(1,1,1)
	btn.TextSize = 14
	local pad = Instance.new("UIPadding", btn)
	pad.PaddingLeft = UDim.new(0, 12)
	pad.PaddingRight = UDim.new(0, 12)

	local content = Instance.new("ScrollingFrame", tabContainer)
	content.Visible = false
	content.Size = UDim2.new(1, 0, 1, 0)
	content.CanvasSize = UDim2.new(0,0,2,0)
	content.BackgroundTransparency = 1
	content.ScrollBarThickness = 6
	tabs[name] = content

	btn.MouseButton1Click:Connect(function()
		for _, f in pairs(tabs) do f.Visible = false end
		content.Visible = true
	end)
end

createTab("LocalPlayer")
createTab("Players")
createTab("Hitbox")
createTab("About")
tabs["LocalPlayer"].Visible = true

-- LOCALPLAYER TAB
do
	local f = tabs["LocalPlayer"]

	local walk = Instance.new("TextLabel", f)
	walk.Text = "WalkSpeed"
	walk.Size = UDim2.new(0,100,0,20)
	walk.Position = UDim2.new(0,10,0,10)
	walk.TextColor3 = Color3.new(1,1,1)
	walk.BackgroundTransparency = 1

	local ws = Instance.new("TextBox", f)
	ws.Text = "16"
	ws.Size = UDim2.new(0,50,0,20)
	ws.Position = UDim2.new(0,120,0,10)

	local wsLoop = true
	task.spawn(function()
		while wsLoop do
			pcall(function()
				lp.Character.Humanoid.WalkSpeed = tonumber(ws.Text)
			end)
			task.wait()
		end
	end)

	local jump = Instance.new("TextLabel", f)
	jump.Text = "JumpPower"
	jump.Size = UDim2.new(0,100,0,20)
	jump.Position = UDim2.new(0,10,0,40)
	jump.TextColor3 = Color3.new(1,1,1)
	jump.BackgroundTransparency = 1

	local jp = Instance.new("TextBox", f)
	jp.Text = "50"
	jp.Size = UDim2.new(0,50,0,20)
	jp.Position = UDim2.new(0,120,0,40)

	local jpLoop = true
	task.spawn(function()
		while jpLoop do
			pcall(function()
				lp.Character.Humanoid.JumpPower = tonumber(jp.Text)
			end)
			task.wait()
		end
	end)

	local infJump = false
	local debounce = false

	local infBtn = Instance.new("TextButton", f)
	infBtn.Text = "Inf Jump [OFF]"
	infBtn.Size = UDim2.new(0, 100, 0, 30)
	infBtn.Position = UDim2.new(0,10,0,80)
	infBtn.MouseButton1Click:Connect(function()
		infJump = not infJump
		infBtn.Text = infJump and "Inf Jump [ON]" or "Inf Jump [OFF]"
	end)

	game:GetService("UserInputService").JumpRequest:Connect(function()
		if infJump and not debounce and lp.Character and lp.Character:FindFirstChild("Humanoid") then
			debounce = true
			lp.Character.Humanoid:ChangeState(Enum.HumanoidStateType.Jumping)
			task.delay(0.16, function() debounce = false end)
		end
	end)

	local noanim = Instance.new("TextButton", f)
	noanim.Text = "No Anim"
	noanim.Size = UDim2.new(0, 100, 0, 30)
	noanim.Position = UDim2.new(0,10,0,120)
	noanim.MouseButton1Click:Connect(function()
		pcall(function()
			lp.Character:FindFirstChild("Animate"):Destroy()
		end)
	end)

	local frst = Instance.new("TextButton", f)
	frst.Text = "Force Reset"
	frst.Size = UDim2.new(0, 100, 0, 30)
	frst.Position = UDim2.new(0,10,0,160)
	frst.MouseButton1Click:Connect(function()
		pcall(function()
			lp.Character.Humanoid.Health = 0
		end)
	end)
end

-- PLAYERS TAB
do
 local f = tabs["Players"]

 local box = Instance.new("TextBox", f)
 box.PlaceholderText = "Nhập tên"
 box.Size = UDim2.new(0, 200, 0, 30)
 box.Position = UDim2.new(0,10,0,10)

 box.FocusLost:Connect(function()
  for _,v in pairs(game.Players:GetPlayers()) do
   if v.Name:lower():find(box.Text:lower()) then
    game.StarterGui:SetCore("SendNotification", {
     Title = "Tìm thấy player",
     Text = v.Name,
     Duration = 2
    })
    break
   end
  end
 end)

 local tp = Instance.new("TextButton", f)
 tp.Text = "TP to Target"
 tp.Size = UDim2.new(0, 150, 0, 30)
 tp.Position = UDim2.new(0,10,0,50)
 tp.MouseButton1Click:Connect(function()
  for _,v in pairs(game.Players:GetPlayers()) do
   if v.Name:lower():find(box.Text:lower()) then
    if v.Character and v.Character:FindFirstChild("HumanoidRootPart") then
     lp.Character:MoveTo(v.Character.HumanoidRootPart.Position)
    end
   end
  end
 end)

 local looptp = false
 local loopbtn = Instance.new("TextButton", f)
 loopbtn.Text = "LoopTP [OFF]"
 loopbtn.Size = UDim2.new(0, 150, 0, 30)
 loopbtn.Position = UDim2.new(0,10,0,90)
 loopbtn.MouseButton1Click:Connect(function()
  looptp = not looptp
  loopbtn.Text = looptp and "LoopTP [ON]" or "LoopTP [OFF]"
 end)

 game:GetService("RunService").RenderStepped:Connect(function()
  if looptp then
   for _,v in pairs(game.Players:GetPlayers()) do
    if v.Name:lower():find(box.Text:lower()) then
     if v.Character and v.Character:FindFirstChild("HumanoidRootPart") then
      lp.Character:MoveTo(v.Character.HumanoidRootPart.Position + Vector3.new(0,2,0))
     end
    end
   end
  end
 end)

 local flingBtn = Instance.new("TextButton", f)
 flingBtn.Text = "Fling Target"
 flingBtn.Size = UDim2.new(0, 150, 0, 30)
 flingBtn.Position = UDim2.new(0,10,0,130)
 local flingActive = false

 flingBtn.MouseButton1Click:Connect(function()
  if flingActive then return end
  flingActive = true
  flingBtn.Text = "Fling [RUNNING]"

  local targetChar = nil
  for _, v in pairs(game.Players:GetPlayers()) do
   if v ~= lp and v.Name:lower():find(box.Text:lower()) then
    targetChar = v.Character
    break
   end
  end

  if targetChar and targetChar:FindFirstChild("HumanoidRootPart") and lp.Character and lp.Character:FindFirstChild("HumanoidRootPart") then
   local myhrp = lp.Character.HumanoidRootPart
   local targetHRP = targetChar.HumanoidRootPart

   local startTime = tick()
   local conn
   conn = game:GetService("RunService").RenderStepped:Connect(function()
    if tick() - startTime >= 2 then
     conn:Disconnect()
     game.StarterGui:SetCore("SendNotification", {
      Title = "FLINGED",
      Text = targetChar.Name,
      Duration = 2
     })
     task.wait(0.3)
     pcall(function()
      lp.Character:FindFirstChildOfClass("Humanoid").Health = 0
     end)
     flingActive = false
     flingBtn.Text = "Fling Target"
     return
    end

    pcall(function()
     myhrp.CFrame = targetHRP.CFrame * CFrame.new(0, 0, 0.5)
     myhrp.Velocity = Vector3.new(9999, 9999, 9999)

     local bv = Instance.new("BodyVelocity", targetHRP)
     bv.Velocity = (targetHRP.Position - myhrp.Position).Unit * 500 + Vector3.new(0, 250, 0)
     bv.MaxForce = Vector3.new(9e9, 9e9, 9e9)
     game.Debris:AddItem(bv, 0.2)
    end)
   end)
  else
   game.StarterGui:SetCore("SendNotification", {
    Title = "Lỗi mẹ rồi",
    Text = "Không tìm thấy target hoặc char lỗi",
    Duration = 2
   })
   flingActive = false
   flingBtn.Text = "Fling Target"
  end
 end)

 local aimlock = false
 local aimBtn = Instance.new("TextButton", f)
 aimBtn.Text = "AIMLOCK [OFF]"
 aimBtn.Size = UDim2.new(0, 150, 0, 30)
 aimBtn.Position = UDim2.new(0,10,0,170)
 aimBtn.MouseButton1Click:Connect(function()
  aimlock = not aimlock
  aimBtn.Text = aimlock and "AIMLOCK [ON]" or "AIMLOCK [OFF]"
 end)

 local cam = workspace.CurrentCamera

 local function getNearestPlayer()
  local closest, dist = nil, math.huge
  for _, plr in pairs(game.Players:GetPlayers()) do
   if plr ~= lp and plr.Character and plr.Character:FindFirstChild("Head") then
    local d = (plr.Character.Head.Position - lp.Character.Head.Position).Magnitude
    if d < dist then
     local ray = Ray.new(cam.CFrame.Position, (plr.Character.Head.Position - cam.CFrame.Position).Unit * 999)
     local hit = workspace:FindPartOnRay(ray, lp.Character, false, true)
     if hit and hit:IsDescendantOf(plr.Character) then
      closest = plr.Character.Head
      dist = d
     end
    end
   end
  end
  return closest
 end

 game:GetService("RunService").RenderStepped:Connect(function()
  if aimlock then
   local target = getNearestPlayer()
   if target then
    cam.CFrame = CFrame.new(cam.CFrame.Position, target.Position)
   end
  end
 end)
end



-- HITBOX TAB
do
	local f = tabs["Hitbox"]
	_G.HeadSize = 100
	_G.LoopHitbox = false
	local hitboxVisual = true

	local slider = Instance.new("TextBox", f)
	slider.Text = tostring(_G.HeadSize)
	slider.Size = UDim2.new(0,100,0,30)
	slider.Position = UDim2.new(0,10,0,10)
	slider.FocusLost:Connect(function()
		_G.HeadSize = tonumber(slider.Text)
	end)

	local toggle = Instance.new("TextButton", f)
	toggle.Text = "Loop Hitbox [OFF]"
	toggle.Size = UDim2.new(0,100,0,30)
	toggle.Position = UDim2.new(0,10,0,50)
	toggle.MouseButton1Click:Connect(function()
		_G.LoopHitbox = not _G.LoopHitbox
		toggle.Text = _G.LoopHitbox and "Loop Hitbox [ON]" or "Loop Hitbox [OFF]"
	end)

	local vis = Instance.new("TextButton", f)
	vis.Text = "Visual Hitbox [ON]"
	vis.Size = UDim2.new(0,120,0,30)
	vis.Position = UDim2.new(0,10,0,90)
	vis.MouseButton1Click:Connect(function()
		hitboxVisual = not hitboxVisual
		vis.Text = hitboxVisual and "Visual Hitbox [ON]" or "Visual Hitbox [OFF]"
	end)

	local Players = game:GetService('Players')
	local RunService = game:GetService('RunService')
	RunService.RenderStepped:Connect(function()
		if not _G.LoopHitbox then return end
		for _, v in pairs(Players:GetPlayers()) do
			if v ~= lp and v.Character and v.Character:FindFirstChild("HumanoidRootPart") then
				local hrp = v.Character.HumanoidRootPart
				pcall(function()
					hrp.Size = Vector3.new(_G.HeadSize, _G.HeadSize, _G.HeadSize)
					hrp.CanCollide = false
					if hitboxVisual then
						hrp.Transparency = 0.7
						hrp.BrickColor = BrickColor.new("Medium stone grey")
						hrp.Material = Enum.Material.Neon
					else
						hrp.Transparency = 1
						hrp.Material = Enum.Material.Plastic
					end
				end)
			end
		end
	end)
end

-- ABOUT TAB
do
	local f = tabs["About"]
	local info = Instance.new("TextLabel", f)
	info.Text = "By C5least\nHope u enjoy it"
	info.Size = UDim2.new(1, 0, 0, 60)
	info.Position = UDim2.new(0, 0, 0, 10)
	info.TextColor3 = Color3.new(1,1,1)
	info.BackgroundTransparency = 1
	info.TextScaled = true
end
