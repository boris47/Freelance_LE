
function Script:Start()
	
	self.shader = Shader:Load("Shaders/PostEffects/00_shaders/_suneffects.shader");
	if ( Level ) then
		Level():WeatherManager():SetSunShader( self.shader );
	else
		self.pSun = nil;
		self.pLight = nil;
	end
end

function Script:Render( camera, context, buffer, depth, diffuse, normals, emission )

	buffer:Enable();
	diffuse:Bind(1);
	depth:Bind(3);
	self.shader:Enable();
		
	if ( not Level ) then -- in editor
		if ( self.pSun == nil  ) then
			local world = camera.world;
			for i = 0, world:CountEntities( Object.DirectionalLightClass ) - 1 do
				local entity = world:GetEntity( i, Object.DirectionalLightClass );
				self.pSun = entity;
				break;
			end
			for i = 0, world:CountEntities( Object.SpotLightClass ) - 1 do
				local entity = world:GetEntity( i, Object.SpotLightClass );
				self.pLight = entity;
				break;
			end
		end
		
		local vSunRotation = Vec3( -0.252852, -0.781738, 0.570045 );
		local vColor = Vec4( 1.000000, 0.929688, 0.851563, 1.0 );
		local vSunPosition = camera:GetPosition( true ) - ( vSunRotation * 150.0 );
		local vLightPosition = camera:GetPosition( true ) - ( vSunRotation * 50.0 );
		
		self.pSun:AlignToVector( vSunRotation );
		self.pSun:SetColor( vColor );
		
		self.pLight:SetPosition( vLightPosition );
		self.pLight:AlignToVector( vSunRotation );
		
		
		local vLightPos = camera:Project( vSunPosition ):xy();
		local vLightDir = Transform:Vector( 0, 0, 1, self.pSun, camera );
		vLightPos.x = vLightPos.x / buffer:GetWidth();
		vLightPos.y = vLightPos.y / buffer:GetHeight();
		
		self.shader:SetVec2( "vLightPos", vLightPos );
		self.shader:SetVec3( "vSunRotation", vLightDir );
		self.shader:SetVec3( "vLightColor", vColor:xyz() );
	end
	
	context:DrawRect( 0, 0, buffer:GetWidth(), buffer:GetHeight() );
	
end

function Script:Detach()
	
	if self.shader then
		self.shader:Release();
		self.shader = nil;
	end
	
end