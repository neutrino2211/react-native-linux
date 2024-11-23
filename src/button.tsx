import React from './lib';

export default function Button({children}: {children: string}) {
  return (
    <gtk-button label={children}/>
  )
}