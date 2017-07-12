-----------------------------
-- Underwater shader by Shadmar
-----------------------------
--Called once at start
function Script:Start()
	self.shader = Shader:Load("Shaders/PostEffects/00_shaders/_underwater.shader")
	self.caust = Texture:Load("Materials/Water/caustics.tex")
	self.wobble = Shader:Load("Shaders/PostEffects/wobble.shader")
	self.passt = Shader:Load("Shaders/PostEffects/00_shaders/_passthrough.shader")
	self.world = World:GetCurrent()
end

--Called each time the camera is rendered
function Script:Render(camera,context,buffer,depth,diffuse,normals)

	if ( not self.world:GetWaterMode() ) then
		context:DrawImage(diffuse,0,0,buffer:GetWidth(),buffer:GetHeight())
		return;
	end
	
	local color=self.world:GetWaterColor()
	local height=self.world:GetWaterHeight()
	
	-------------------------------------------------------------
	--adusts for shafts and wavesize,amplitude
	-------------------------------------------------------------
	local use_shafts=camera:GetKeyValue("uw_shafts","1")
	local wavesize	=camera:GetKeyValue("uw_wavesize","0.05")
	local amplitude =camera:GetKeyValue("uw_amplitude","0.005")
	
	
	if self.buffer==nil then
		local w=buffer:GetWidth()
		local h=buffer:GetHeight()
		self.buftex = Texture:Create(buffer:GetWidth(),buffer:GetHeight(),Texture.RGB, 0, 1, 0)
		self.buftex:SetFilter(Texture.Smooth)
		self.buftex:SetClampMode(true,true)
		self.buffer={}
		self.buffer[0]=Buffer:Create(buffer:GetWidth(),buffer:GetHeight())
		self.buffer[0]:SetColorTexture(self.buftex)
	end
	
	if self.shader 
	and self.caust
	and self.wobble
	and self.passt
	and height > 0
	--and camera:GetScale().y==1
	then 
		--eye
		self.dir = Vec3(camera.mat.k.x,camera.mat.k.y,camera.mat.k.z):Normalize()
		self.buffer[0]:Enable()
		self.shader:Enable() 
		diffuse:Bind(1)
		depth:Bind(2)
		normals:Bind(3)
		self.shader:SetVec4("fogcolor",color)
		self.caust:Bind(4)
		self.shader:SetVec3("eye",self.dir)
		self.shader:SetFloat("level",height)
		self.shader:SetInt("useshafts",use_shafts);
		if camera:GetScale().y==1 and camera:GetPosition().y <= height
		then
			self.shader:SetInt("isunderwater",1)
		else
			self.shader:SetInt("isunderwater",0)
		end
		context:DrawImage(diffuse,0,0,buffer:GetWidth(),buffer:GetHeight())
	end
	--if camera:GetPosition().y < height and camera:GetPosition().y > height-.1 
	--camera:SetScale(1,1,1)
	buffer:Enable()
	if height > 0
	then
		self.buffer[0]:GetColorTexture():Bind(1)
	else
		diffuse:Bind(1)
	end
	
	if camera:GetScale().y==1 and camera:GetPosition().y < height
	then
		--DOF underwater (if used)
		--save dof defaults
		local fringe=camera:GetKeyValue("dof_fringe","5.0")
		local ndofstart=camera:GetKeyValue("dof_ndofstart","0.0")
		local ndofdist=camera:GetKeyValue("dof_ndofdist","3.0")
		local fdofstart=camera:GetKeyValue("dof_fdofstart","0.0")
		local fdofdist=camera:GetKeyValue("dof_fdofdist","500.0")
		local vignetting=camera:GetKeyValue("dof_vignetting","0")
		--save bloom
		local Luminance=camera:GetKeyValue("bloom_Luminance","0.1")
		local fMiddleGray=camera:GetKeyValue("bloom_MiddleGray","0.48")
		local fWhiteCutoff=camera:GetKeyValue("bloom_WhiteCutoff","0.98")
		
		--------------------------------------------------------
		--adjusts for underwater dof (tweak here if dof is in your stack):
		--------------------------------------------------------
		camera:SetKeyValue("dof_fringe","10.0")
		camera:SetKeyValue("dof_ndofstart","0.0")
		camera:SetKeyValue("dof_ndofdist","1.5")
		camera:SetKeyValue("dof_fdofstart","0.0")
		camera:SetKeyValue("dof_fdofdist","25.0")
		camera:SetKeyValue("dof_vignetting","0")
		
		--------------------------------------------------------
		--adusts for underwater bloom (tweak here if 08_bloom* is in your stack):
		--------------------------------------------------------
		camera:SetKeyValue("bloom_Luminance","0.0925")
		camera:SetKeyValue("bloom_MiddleGray","0.54")
		camera:SetKeyValue("bloom_WhiteCutoff","0.98")
		--------------------------------------------------
		
		self.wobble:Enable()
		self.wobble:SetFloat("amplitude",amplitude);
		self.wobble:SetFloat("wavesize",wavesize);
		context:DrawImage(self.buffer[0]:GetColorTexture(),0,0,buffer:GetWidth(),buffer:GetHeight())
	else
		self.passt:Enable()
		-- DOF reset (if used)
		camera:SetKeyValue("dof_fringe",fringe)
		camera:SetKeyValue("dof_ndofstart",ndofstart)
		camera:SetKeyValue("dof_ndofdist",ndofdist)
		camera:SetKeyValue("dof_fdofstart",fdofstart)
		camera:SetKeyValue("dof_fdofdist",fdofdist)
		camera:SetKeyValue("dof_vignetting",vignetting)
		--bloom reset (if used)
		camera:SetKeyValue("bloom_Luminance",Luminance)
		camera:SetKeyValue("bloom_MiddleGray",fMiddleGray)
		camera:SetKeyValue("bloom_WhiteCutoff",fWhiteCutoff)
		
		context:DrawRect(0,0,buffer:GetWidth(),buffer:GetHeight())
	end
end

--Called when the effect is detached or the camera is deleted
function Script:Detach()
	if self.shader then
		self.shader:Release()
		self.caust:Release()
		self.wobble:Release()
		self.shader = nil
	end
	if self.buffer~=nil then
		self.buffer[0]:Release()
	end
end