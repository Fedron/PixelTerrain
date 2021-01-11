sampler2D lightness;

void main()
{
    vec4 pixel = texture2D(lightness, gl_TexCoord[0].xy);

    vec4 color = vec4(
        gl_Color.r * pixel.r,
        gl_Color.g * pixel.g,
        gl_Color.b * pixel.b,
        gl_Color.a
    );

    gl_FragColor = color;
}