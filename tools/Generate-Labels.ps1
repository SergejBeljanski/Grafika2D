param()
Add-Type -AssemblyName System.Drawing
$root = Split-Path $PSScriptRoot -Parent
function New-TextTexture($name, $width, $height, $lines) {
    $bitmap = [System.Drawing.Bitmap]::new($width, $height)
    $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
    $graphics.Clear([System.Drawing.Color]::Transparent)
    $graphics.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::AntiAliasGridFit
    foreach ($line in $lines) {
        $font = [System.Drawing.Font]::new('Segoe UI', [float]$line.Size, [System.Drawing.FontStyle]$line.Style, [System.Drawing.GraphicsUnit]::Pixel)
        $brush = [System.Drawing.SolidBrush]::new([System.Drawing.ColorTranslator]::FromHtml($line.Color))
        $graphics.DrawString($line.Text, $font, $brush, [float]$line.X, [float]$line.Y)
        $brush.Dispose(); $font.Dispose()
    }
    $bitmap.Save((Join-Path $root "Resources/$name.png"), [System.Drawing.Imaging.ImageFormat]::Png)
    $graphics.Dispose(); $bitmap.Dispose()
}
New-TextTexture 'student' 990 152 @(
    @{Text='Sergej Beljanski'; Size=42; Style='Bold'; Color='#E7F5EB'; X=15; Y=6},
    @{Text='RA 202/2021   /   RACUNARSKA GRAFIKA'; Size=25; Style='Regular'; Color='#A7D0CA'; X=17; Y=77}
)

