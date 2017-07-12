
function Script:Start()

	self.shader = Shader:Load("Shaders/PostEffects/00_shaders/_klepto_fog.shader")
	if ( Level ) then
		Level():WeatherManager():SetFogShader( self.shader );
	end
	
end

function Script:Render(camera,context,buffer,depth,diffuse,normals)

	self.shader:Enable()
	diffuse:Bind(1);
	depth:Bind(3)
	context:DrawImage( diffuse, 0, 0, buffer:GetWidth(), buffer:GetHeight() )
	
end

function Script:Detach()

	if self.shader then
		self.shader:Release()
		self.shader = nil
	end
	
end