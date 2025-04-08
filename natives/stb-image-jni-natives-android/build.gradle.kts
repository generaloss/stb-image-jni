plugins {
    id("com.android.library") version "8.1.4"
    id("maven-publish")
    id("signing")
}

android {
    namespace = "generaloss.stb.image"
    compileSdk = 34

    defaultConfig {
        minSdk = 21
    }

    sourceSets["main"].jniLibs.srcDirs("src/main/jniLibs")

    publishing {
        singleVariant("release") {
            withSourcesJar()
        }
    }
}

afterEvaluate {
    publishing {
        publications {
            create<MavenPublication>("release") {
                from(components["release"])

                groupId = "io.github.generaloss"
                artifactId = "stb-image-jni-natives-android"
                version = "25.4.1"

                pom {
                    name.set("stb-image-jni-natives-android")
                    description.set("STB-Image Library JNI android native libraries")
                    url.set("https://github.com/generaloss/stb-image-jni")

                    licenses {
                        license {
                            name.set("MIT License")
                            url.set("https://mit-license.org/")
                        }
                        license {
                            name.set("Public Domain License")
                            url.set("https://creativecommons.org/public-domain/")
                        }
                    }

                    developers {
                        developer {
                            id.set("generaloss")
                            name.set("Pavel Vladimirovich")
                            email.set("generaloss@proton.me")
                        }
                    }

                    scm {
                        connection.set("scm:git:git://github.com/generaloss/stb-image-jni.git")
                        developerConnection.set("scm:git:ssh://github.com:generaloss/stb-image-jni.git")
                        url.set("https://github.com/generaloss/stb-image-jni/tree/master/natives/stb-image-jni-natives-android")
                    }
                }
            }
        }

        repositories {
            maven {
                name = "localTest"
                url = uri("${rootProject.buildDir}/repo")
            }
        }
    }
    signing {
        useGpgCmd()
        sign(publishing.publications["release"])
    }
}