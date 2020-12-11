module GeometryGenPipeline =

	let attentuate value dist lightParams =
		value / (dot dist lightParams)
		

	let specularFactor  lightDir Normal ViewDir FragPos =

		lightDir + viewDir 
		|> dot Normal
		|> max 0.0
		|> pow calcExponent


	let diffuseFactor lightDir Normal = 
		lightDir
		|> normalize
        |> dot Normal
		|> max 0.0
	
	let totalColor light Normal

		