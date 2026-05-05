$VCPKGDir = "vcpkg"
$RepoURL  = "https://github.com/microsoft/vcpkg.git"

Write-Host "=== Setting up vcpkg ==="

if (-Not (Test-Path $VCPKGDir)) {
    Write-Host "Cloning vcpkg into '$VCPKGDir'..."
    git clone $RepoURL $VCPKGDir
} else {
    Write-Host "vcpkg already exits at '$VCPKGDir'"
}

Set-Location $VCPKGDir

Write-Host "Bootstrapping vcpkg..."
.\bootstrap-vcpkg.bat

Set-Location ..

Write-Host "=== vcpkg setup complete ==="
