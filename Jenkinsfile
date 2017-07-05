void pullCode(String dir) {
    checkout poll: false, 
    	scm: [$class: "GitSCM", 
    	branches: [[name: "*/dev"]], doGenerateSubmoduleConfigurations: false,  
    	extensions: [[$class: "RelativeTargetDirectory", relativeTargetDir: dir]], 
    	submoduleCfg: [], 
    	userRemoteConfigs: [[url: "https://github.com/unihd-cag/fsmdesigner.git"]]]
}

stage("Debian") {
    
    node("debian") {
        
        target = "debian"
        pullCode(target)
        sh "mkdir -p $target/build"
        sh "cd $target/build/ && cmake .. -G 'Unix Makefiles'"
        sh "cd $target/build && make"
        sh "cd $target/build && make package"
        archiveArtifacts artifacts: "$target/build/*.deb", onlyIfSuccessful: true
    }
    
}

/*stage("Mingw") {
    
    node("nsis") {
        
        target = "mingw"
        pullCode(target)
        sh "mkdir -p $target/build"
        sh "cd $target/build/ && cmake .. -G 'Unix Makefiles' -DCMAKE_TOOLCHAIN_FILE=./macros/CC_WIN64_MINGW.cmake"
        sh "cd $target/build && make"
        sh "cd $target/build && make package"
        archiveArtifacts artifacts: "$target/build/*.exe", onlyIfSuccessful: true
    }
    
    
}*/