--------------------------------------------------------
-- DOF shader translated by Shadmar
-- by Martins Upitis (martinsh) (devlog-martinsh.blogspot.com)
--------------------------------------------------------
--Called once at start
function Script:Start()
	self.shader = Shader:Load("Shaders/PostEffects/00_shaders/_dof.shader")
	self.shader_pass = Shader:Load("Shaders/PostEffects/00_shaders/_passthrough.shader")
end

--Called each time the camera is rendered
function Script:Render(camera,context,buffer,depth,diffuse,normals)
	--Brightpass
	
	if (
		self.shader
		and self.shader_pass
		and camera:GetScale().y==1
		) then
		
		self.shader:Enable()

		--adjust for dof ranges, options
		local ndofstart=camera:GetKeyValue("dof_ndofstart","0.0")
		local ndofdist=camera:GetKeyValue("dof_ndofdist","3.0")
		local fdofstart=camera:GetKeyValue("dof_fdofstart","0.0")
		local fdofdist=camera:GetKeyValue("dof_fdofdist","500.0")

		local vignetting=camera:GetKeyValue("dof_vignetting","0")
		local vignout=camera:GetKeyValue("dof_vignout","1.3")
		local vignin=camera:GetKeyValue("dof_vignin","-0.0")
		local vignfade=camera:GetKeyValue("dof_vignfade","180.0")

		local maxblur=camera:GetKeyValue("dof_maxblur","2.0")

		local threshold=camera:GetKeyValue("dof_threshold","0.25")
		local gain=camera:GetKeyValue("dof_gain","1.0")

		local bias=camera:GetKeyValue("dof_bias","0.5")
		local fringe=camera:GetKeyValue("dof_fringe","5.0")

		local namount=camera:GetKeyValue("dof_namount","0.0001")

		self.shader:SetFloat("ndofstart",ndofstart)
		self.shader:SetFloat("ndofdist",ndofdist)
		self.shader:SetFloat("fdofstart",fdofstart)
		self.shader:SetFloat("fdofdist",fdofdist)

		self.shader:SetInt("vignetting",vignetting)
		self.shader:SetFloat("vignout",vignout)
		self.shader:SetFloat("vignin",vignin)
		self.shader:SetFloat("vignfade",vignfade)

		self.shader:SetFloat("maxblur",maxblur)

		self.shader:SetFloat("threshold",threshold)
		self.shader:SetFloat("gain",gain)

		self.shader:SetFloat("bias",bias)
		self.shader:SetFloat("fringe",fringe)

		self.shader:SetFloat("namount",namount)

		diffuse:Bind(1)
		depth:Bind(2)
		context:DrawImage(diffuse,0,0,buffer:GetWidth(),buffer:GetHeight())
	else
		buffer:Enable()
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
	if self.shader_pass then
		self.shader_pass:Release()
		self.shader_pass = nil
	end
end