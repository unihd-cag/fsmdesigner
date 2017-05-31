node("debian") {

	stage("Compile") {

		sh "mkdir -p build"
		sh 'cd build/ && cmake .. -G "Unix Makefiles"'
		sh 'cd build && make'

	}

	stage("Package") {
		sh 'cd build && make package'
	}

}
node("linux") {

	stage("Create Project using Toolchain File") {
		
	}
}
