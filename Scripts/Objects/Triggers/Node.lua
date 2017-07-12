Script.pPointA		= nil;			--Entity "Point A"
Script.pPointB		= nil;			--Entity "Point B"

Script.pEntity		= nil;
Script.bLinked		= false;


function Script:Start()
	if ( ( self.pPointA == nil ) or ( self.pPointB == nil ) ) then
		Debug:Error( "Node "  .. self.entity:GetKeyValue( "name" ) .. " need a Point A and a Point B" );
		return;
	end
end


function Script:Reset()
	self.pEntity = nil;
	self.bLinked = false;
end


function Script:GetName() return self.entity:GetKeyValue( "name" ); end


function Script:Exit()
	self.pPointA.script:Exit();
	self.pPointB.script:Exit();
	self:Reset();
end


-- Make a switch to rotate entity movement to Point2, disabling Point1
function Script:SwitchFromTo( Point1, Point2 )
	
	Point1.script:Reset();
	
	-- Set Entity position and rotation
	local vFromPToEntity = Point2:GetPosition() - self.entity:GetPosition();
	if ( InputManager.Forward() ) then
		self.pEntity:SetRotationP1ToP2( vFromPToEntity );
		
		-- Unset flag, if present, for Point1 as destination
		if ( type( Point1.script.bDestination ) == "boolean" ) then
			Point1.script.bDestination = false;
		end
		
		-- Set flag, if present, for Point2 as destination
		if ( type( Point2.script.bDestination ) == "boolean" ) then
			Point2.script.bDestination = true;
		end
	end
	
	if ( InputManager.Backward() ) then
		self.pEntity:SetRotationP1ToP2( vFromPToEntity:Inverse() );
		
		-- Set flag, if present, for Point1 as destination
		if ( type( Point1.script.bDestination ) == "boolean" ) then
			Point1.script.bDestination = true;
		end
		
		-- Unset flag, if present, for Point2 as destination
		if ( type( Point2.script.bDestination ) == "boolean" ) then
			Point2.script.bDestination = false;
		end
	end
	
	self.pEntity:GetEntity():SetPosition( self.entity:GetPosition() );
	
end

function Script:DestPointReached( Point )

	if ( type( Point.script.bDestination ) == "boolean" ) then
		if ( Point.script.bDestination ) then
			local pEntity = self.pEntity:GetEntity(); -- Leadwerks Entity
			if ( pEntity:GetDistance( Point ) < Point.script.fOutRadius ) then
				return true;
			end
		end
	end
	
	return false;

end

function Script:CheckSwitchPoint()
	
	-- if distance between user and previous point is more then the one between the next point
	local pEntity = self.pEntity:GetEntity(); -- user, Leadwerks Entity
	if ( self.pPointA.script.bLinked ) then	-- can arrive from point A
		
		if ( self:DestPointReached( self.pPointA ) ) then
			self:Exit();
		end
		
		-- if can, switch to point B
		if ( pEntity:GetDistance( self.pPointA ) > self.entity:GetDistance( self.pPointA ) ) then
			
			self:SwitchFromTo( self.pPointA, self.pPointB );
			self.pPointB.script.pEntity = self.pEntity;
			self.pPointB.script.bLinked = true;
			
		end
	end
	
	if ( self.pPointB.script.bLinked ) then	-- can arrive from point B
		
		if ( self:DestPointReached( self.pPointB ) ) then
			self:Exit();
		end
		
		-- if can, switch to point A
		if ( pEntity:GetDistance( self.pPointB ) > self.entity:GetDistance( self.pPointB ) ) then
			
			self:SwitchFromTo( self.pPointB, self.pPointA );
			self.pPointA.script.pEntity = self.pEntity;
			self.pPointA.script.bLinked = true;
			return
		end
	end
	
end


function Script:FixedUpdate()
	
	-- skip if has not entity
	if ( self.pEntity == nil ) then return; end
	
	-- when user use jump command to exit and return to plain walk motion
	if ( InputManager.Jump() ) then
		
		-- now as cEntity
		self.pEntity:As"LiveEntity":SetPlainWalking();
		
		-- add a forward force
		local vDirection = Transform:Vector( 0, 250, 800, self.pEntity:GetEntity(), nil );
		self.pEntity:GetEntity():AddForce( vDirection );
		self:Exit();
		return;
		
	end
	
	-- Rotation switch check
	self:CheckSwitchPoint();
	
end
