void pullCode(String dir) {
    checkout poll: false, 
    	scm: [$class: "GitSCM", 
    	branches: [[name: "*/dev"]], doGenerateSubmoduleConfigurations: false,  
    	extensions: [[$class: "RelativeTargetDirectory", relativeTargetDir: dir]], 
    	submoduleCfg: [], 
    	userRemoteConfigs: [[url: "https://github.com/unihd-cag/fsmdesigner.git"]]]
}


stage("Debian Source Package") {
    
    node("debian") {
        pullCode("source")
        sh "cd source && make deb-src"
        archiveArtifacts artifacts: ["source/.deb/*.dsc","source/.deb/*.changes","source/.deb/*.xy"], onlyIfSuccessful: true
    }
}

stage("Jessie Backports") {
    node("debian") {
        sh "rm -f source/.deb./*.deb"
        sh "DISTRIBUTION=jessie-backports ARCHITECTURE=amd64 cd source && make deb-build"
        archiveArtifacts artifacts: ["source/.deb/*.deb"], onlyIfSuccessful: true
    }
}

stage("Testing") {
    node("debian") {
        sh "rm -f source/.deb./*.deb"
        sh "DISTRIBUTION=testing ARCHITECTURE=amd64 cd source && make deb-build"
        archiveArtifacts artifacts: ["source/.deb/*.deb"], onlyIfSuccessful: true
    }
}

/*stage("Debian") {
    
    node("debian") {
        
        target = "debian"
        pullCode(target)
        sh "mkdir -p $target/build"
        sh "rm -Rf $target/build/*.deb"
        sh "cd $target/build/ && cmake .. -G 'Unix Makefiles'"
        sh "cd $target/build && make"
        sh "cd $target/build && make package"
        archiveArtifacts artifacts: "$target/build/*.deb", onlyIfSuccessful: true
    }
    
}*/

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