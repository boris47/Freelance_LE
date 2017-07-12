-------------------------------------------------
-- Hack by shadmar, use alpha blend to fake iris opening and HDR tonemap
-------------------------------------------------
--Called once at start
function Script:Start()
	self.shader = Shader:Load("Shaders/PostEffects/00_shaders/_hdr_tonemap.shader")
	self.pass = Shader:Load("Shaders/PostEffects/00_shaders/_hdr_combine.shader")
	self.shader_pass = Shader:Load("Shaders/PostEffects/00_shaders/_passthrough.shader")
end

--Called each time the camera is rendered
function Script:Render(camera,context,buffer,depth,diffuse,normals)
	
	if self.buffer==nil then
		self.buffer=Buffer:Create(buffer:GetWidth()/2,buffer:GetHeight()/2)
		self.buffer:GetColorTexture():SetFilter(Texture.Smooth)
	end
	
	--adjust gain, less is more
	local gain=camera:GetKeyValue("hdr_gain","1.25") -- less is more
	local expose=camera:GetKeyValue("hdr_expose","1.0") -- bloom will set this to 0.6
	local clampiris=camera:GetKeyValue("hdr_clampiris","0.0,2.0") -- bloom will set this to 0.8,2.0

	if self.shader
	and self.pass
	and self.shader_pass
	and camera:GetScale().y==1
	then
		self.buffer:Enable()
		self.shader:Enable()
		diffuse:Bind(1)
		self.shader:SetFloat("appspeed",Time:GetSpeed())
		context:SetBlendMode(Blend.Alpha)
		context:DrawImage(diffuse,0,0,buffer:GetWidth(),buffer:GetHeight())
		context:SetBlendMode(Blend.Solid)

		buffer:Enable()
		self.buffer:GetColorTexture():Bind(1)
		diffuse:Bind(2)
		self.pass:Enable()
		self.pass:SetFloat("gain",gain)
		self.pass:SetFloat("expose",expose)
		self.pass:SetVec2("clampiris",Vec2(clampiris))
		context:DrawImage(diffuse,0,0,buffer:GetWidth(),buffer:GetHeight())
	else
		if self.shader_pass then self.shader_pass:Enable() end
		context:DrawRect(0,0,buffer:GetWidth(),buffer:GetHeight())
	end
	
end

--Called when the effect is detached or the camera is deleted
function Script:Detach()
	if self.shader then
		self.shader:Release()
		self.shader = nil
	end
	if self.shader then
		self.pass:Release()
		self.pass = nil
	end
	if self.shader_pass then
		self.shader_pass:Release()
		self.shader_pass = nil
	end
	if self.buffer~=nil then
		self.buffer[0]:Release()
	end

end