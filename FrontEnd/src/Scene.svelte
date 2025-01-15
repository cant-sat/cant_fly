<script>
	import { Canvas, OrbitControls, T } from '@threlte/core'
	import { degToRad } from 'three/src/math/MathUtils'
</script>

<div class="min-h-[100dvh]">
	<Canvas>
		<T.PerspectiveCamera makeDefault position={[10, 10, 10]} fov={24}>
			<OrbitControls
				maxPolarAngle={degToRad(70)}
				enableZoom={false}
				target={{ y: 0.5 }}
			/>
		</T.PerspectiveCamera>

		<T.DirectionalLight castShadow position={[3, 10, 10]} />
		<T.DirectionalLight position={[-3, 10, -10]} intensity={0.2} />
		<T.AmbientLight intensity={0.2} />

		<!-- cansat -->
		<T.Group position.y={0.5}>
			<T.Mesh  castShadow let:ref>
				
				
				<T.CylinderGeometry args={[0.5, 0.5, 2]} />
				<T.MeshStandardMaterial color="#333333" />
			</T.Mesh>
		</T.Group>

		<!-- Floor -->
		<T.Mesh receiveShadow rotation.x={degToRad(-90)}>
			<T.CircleGeometry args={[3, 72]} />
			<T.MeshNormalMaterial color="white" />
		</T.Mesh>
	</Canvas>
</div>
