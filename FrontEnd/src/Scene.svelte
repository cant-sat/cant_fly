<script>
	import { Canvas, InteractiveObject, OrbitControls, T } from '@threlte/core'
	import { spring } from 'svelte/motion'
	import { degToRad } from 'three/src/math/MathUtils'

	const scale = spring(1)
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

		<!-- Cube -->
		<T.Group scale={$scale}>
			<T.Mesh position.y={0.5} castShadow let:ref>
				
				[radiusTop?: number, radiusBottom?: number, height?: number, radialSegments?: number, heightSegments?: number, openEnded?: boolean, thetaStart?: number, thetaLength?: number]
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
